#include <viarmanoisedetector.h>

#define TWO_PI 6.2831853071795864769252866
#define LN2PI1 2.837877066409345 // log(2*pi) + 1

#define DEFAULT_MA_DEGREE 3
#define DEFAULT_AR_DEGREE 7

#define WINDOW_SIZE 2048
#define PREDICTION_SIZE 512

#define DEFAULT_GRETL_CRITERIA BIC

#define MAXIMUM_ITERATIONS 500
#define MAXIMUM_TEST_DEGREE 30

ViArmaNoiseDetector::ViArmaNoiseDetector(const Type &type, const Mode &mode, const GretlEstimation &estimation)
	: ViNoiseDetector()
{
	ViGretl::initialize();

	mWindowData = NULL;
	mMaMatrix = NULL;
	mArMatrix = NULL;
	mMaCoefficients = NULL;
	mArCoefficients = NULL;
	mPartialMatrix = NULL;

	mGretlData = NULL;
	mGretlParameters = NULL;
	mGretlEstimation = OPT_NONE;

	setMode(mode);

	if(type == MA || type == ARMA) setDegree(MA, DEFAULT_MA_DEGREE);
	else setDegree(MA, 0);
	if(type == AR || type == ARMA) setDegree(AR, DEFAULT_AR_DEGREE);
	else setDegree(AR, 0);

	setType(type);
	setWindowSize(WINDOW_SIZE);
	setOffset(mWindowSize);
	if(mMode == Gretl) setGretlEstimation(estimation);

	setGretlCriteria(DEFAULT_GRETL_CRITERIA);
}

ViArmaNoiseDetector::ViArmaNoiseDetector(const ViArmaNoiseDetector &other)
	: ViNoiseDetector(other)
{
	ViGretl::initialize();

	mWindowData = NULL;
	mMaMatrix = NULL;
	mArMatrix = NULL;
	mMaCoefficients = NULL;
	mArCoefficients = NULL;
	mPartialMatrix = NULL;

	mGretlData = NULL;
	mGretlParameters = NULL;
	mGretlEstimation = other.mGretlEstimation;

	setMode(other.mMode);

	if(other.mType == MA || other.mType == ARMA) setDegree(MA, other.mMaDegree);
	else setDegree(MA, 0);
	if(other.mType == AR || other.mType == ARMA) setDegree(AR, other.mArDegree);
	else setDegree(AR, 0);

	setType(other.mType);
	setWindowSize(other.mWindowSize);
	setOffset(mWindowSize);

	setGretlCriteria(other.mGretlCriteria);
}

ViArmaNoiseDetector::~ViArmaNoiseDetector()
{
	if(mWindowData != NULL)
	{
		delete [] mWindowData;
		mWindowData = NULL;
	}
	if(mPartialMatrix != NULL)
	{
		for(int i = 0; i <= mWindowSize; ++i)
		{
			delete [] mPartialMatrix[i];
		}
		delete [] mPartialMatrix;
	}
	clear(ARMA);

	if(mMode == Gretl)
	{
		if(mGretlParameters != NULL)
		{
			free(mGretlParameters);
			mGretlParameters = NULL;
		}
		if(mGretlData != NULL)
		{
			destroy_dataset(mGretlData);
			mGretlData = NULL;
		}
	}
}

QString ViArmaNoiseDetector::name(QString replace, bool spaced)
{
	QString n = ViNoiseDetector::name(replace, spaced);
	if(spaced) n += " ";
	n += QString::number(mMaDegree);
	if(spaced) n += " ";
	else n += "_";
	n += QString::number(mArDegree);
	return n;
}

void ViArmaNoiseDetector::setParameters(qreal param1)
{
	setDegree(mType, param1);
}

void ViArmaNoiseDetector::setParameters(qreal param1, qreal param2)
{
	setDegree(param1, param2);
}

void ViArmaNoiseDetector::setParameters(qreal param1, qreal param2, qreal param3)
{
	setWindowSize(param3);
	setDegree(param1, param2);
}

void ViArmaNoiseDetector::setType(const Type &type)
{
	mType = type;
	if(mMode == Gretl)
	{
		if(mType == MA) mGretlParameters[1] = 0; // Set AR order to 0
		else if(mType == AR) mGretlParameters[3] = 0; // Set MA order to 0
	}
}

void ViArmaNoiseDetector::setMode(const Mode &mode)
{
	mMode = mode;
	if(mode == Native)
	{
		calculateNoisePointer = &ViArmaNoiseDetector::calculateNoiseNative;
		srand((unsigned)time(NULL));
	}
	else if (mode == Gretl)
	{
		calculateNoisePointer = &ViArmaNoiseDetector::calculateNoiseGretl;

		libset_set_int(BFGS_MAXITER, MAXIMUM_ITERATIONS); // ML
		libset_set_int(BHHH_MAXITER, MAXIMUM_ITERATIONS); // CML

		if(mGretlParameters == NULL)
		{
			mGretlParameters = gretl_list_new(5);
			mGretlParameters[1] = 0;        // AR order
			mGretlParameters[2] = 0;        // order of integration
			mGretlParameters[3] = 0;        // MA order
			mGretlParameters[4] = LISTSEP;  // separator
			mGretlParameters[5] = 1;        // position of dependent variable in dataset - 1 because we have 2 sets in mGretlData and the first one (0) is reserved for gretl
		}

		mGretlPredictions.resize(PREDICTION_SIZE);
	}
}

void ViArmaNoiseDetector::setGretlEstimation(const GretlEstimation &estimation)
{
	if(estimation == ExactMaximumLikelihoodLimited) mGretlEstimation = OPT_L;
	else if(estimation == ConditionalMaximumLikelihood) mGretlEstimation = OPT_C;
	else if(estimation == X12Arma)
	{
		mGretlEstimation = OPT_NONE;
		QFile executable(X12AINSTALL);
		bool found = true;
		if(!executable.exists())
		{
			executable.setFileName(X12ABUILD);
			if(!executable.exists())
			{
				found = false;
				LOG("The X12Arima executable cannot be found.", QtFatalMsg);
			}
		}
		if(found)
		{
			mGretlEstimation = OPT_X;
			ConfigPaths cp = {0};
			strcpy(cp.x12a, executable.fileName().toLatin1().data());
			gretl_set_paths(&cp);
		}
	}
	else mGretlEstimation = OPT_NONE;
}

void ViArmaNoiseDetector::setGretlCriteria(const GretlCriteria &criteria)
{
	mGretlCriteria = criteria;
	if(mGretlCriteria == None)
	{
		gretlModelPointer = &ViArmaNoiseDetector::gretlFixedModel;
	}
	else if(mGretlCriteria == AutoCorrelation)
	{
		gretlModelPointer = &ViArmaNoiseDetector::gretlAutocorrelationModel;
	}
	else
	{
		gretlModelPointer = &ViArmaNoiseDetector::gretlBestModel;
		if(mGretlCriteria == R2)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlR2;
		}
		else if(mGretlCriteria == AR2)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlAR2;
		}
		else if(mGretlCriteria == AIC)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlAIC;
		}
		else if(mGretlCriteria == AICC)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlAICC;
		}
		else if(mGretlCriteria == BIC)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlBIC;
		}
		else if(mGretlCriteria == HQC)
		{
			gretlOrderPointer = &ViArmaNoiseDetector::gretlHQC;
		}
	}
}

void ViArmaNoiseDetector::setWindowSize(int size)
{
	if(mMode == Native && mPartialMatrix != NULL)
	{
		for(int i = 0; i <= mWindowSize; ++i)
		{
			delete [] mPartialMatrix[i];
		}
		delete [] mPartialMatrix;
	}

	mWindowSize = size;
	if(mWindowData != NULL)
	{
		delete [] mWindowData;
	}
	mWindowData = new qreal[mWindowSize];

	if(mMode == Native)
	{
		mPartialMatrix = new qreal*[mWindowSize + 1];
		for(int i = 0; i <= mWindowSize; ++i)
		{
			mPartialMatrix[i] = new qreal[mWindowSize + 1];
		}
	}
	else if(mMode == Gretl)
	{
		if(mGretlEstimation == OPT_X && mWindowSize > 600)
		{
			LOG("X12Arima can have a maximum window size of 600. Resetting it 600.", QtCriticalMsg);
			setWindowSize(600);
			return;
		}
		if(mGretlData != NULL) destroy_dataset(mGretlData);
		mGretlData = create_new_dataset(2, mWindowSize + PREDICTION_SIZE, 0); // + PREDICTION_SIZE for out-of-sample prediction
		strcpy(mGretlData->varname[1], "visoredata"); // For X12 we need the series to have a name
		mGretlData->t2 = mGretlData->n - PREDICTION_SIZE - 1; // reserve PREDICTION_SIZE observations for out-of-sample forecasting
	}

	mConfidenceLevel = 1.96 / qSqrt(mWindowSize - 1);
}

void ViArmaNoiseDetector::setDegree(const Type &type, const int &degree)
{
	if(type == MA || type == ARMA)
	{
		clear(MA);
		mMaDegree = degree;

		if(mMode == Native && mMaDegree > 0)
		{
			mMaMatrix = new qreal*[mMaDegree];
			mMaCoefficients = new qreal[mMaDegree];
			for(int i = 0; i < mMaDegree; ++i)
			{
				mMaMatrix[i] = new qreal[mMaDegree];
			}
		}
	}
	if(type == AR || type == ARMA)
	{
		clear(AR);
		mArDegree = degree;

		if(mMode == Native && mArDegree > 0)
		{
			mArMatrix = new qreal*[mArDegree];
			mArCoefficients = new qreal[mArDegree];
			for(int i = 0; i < mArDegree; ++i)
			{
				mArMatrix[i] = new qreal[mArDegree];
			}
		}
	}
}

void ViArmaNoiseDetector::setDegree(const int &maDegree, const int &arDegree)
{
	setDegree(MA, maDegree);
	setDegree(AR, arDegree);
}
/*
qreal ViArmaNoiseDetector::generateNoise(const qreal &variance) const
{
	static bool hasSpare = false;
	static qreal rand1, rand2;

	if(hasSpare)
	{
		hasSpare = false;
		return variance * rand1 * qSin(rand2);
	}

	hasSpare = true;

	rand1 = rand() / ((double) RAND_MAX);
	if(rand1 < 1e-100) rand1 = 1e-100;
	rand1 = qSqrt(-2 * qLn(rand1));
	rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;

	return variance * rand1 * qCos(rand2);
}*/

qreal ViArmaNoiseDetector::generateNoise(const qreal &variance) const
{
	static bool hasSpare = false;
	static qreal spare;

	if(hasSpare)
	{
		hasSpare = false;
		return spare * variance;
	}

	hasSpare = true;
	static qreal u, v, s;
	do
	{
		u = (rand() / ((double) RAND_MAX)) * 2 - 1;
		v = (rand() / ((double) RAND_MAX)) * 2 - 1;
		s = u * u + v * v;
	}
	while(s >= 1 || s == 0);

	s = qSqrt(-2.0 * qLn(s) / s);
	spare = v * s;
	return variance * u * s;
}

void ViArmaNoiseDetector::clear(const Type &type)
{
	if(type == MA || type == ARMA)
	{
		if(mMaCoefficients != NULL)
		{
			delete [] mMaCoefficients;
			mMaCoefficients = NULL;
		}
		if(mMaMatrix != NULL)
		{
			for(int i = 0; i < mMaDegree; ++i)
			{
				delete [] mMaMatrix[i];
			}
			delete [] mMaMatrix;
			mMaMatrix = NULL;
		}
	}

	if(type == AR || type == ARMA)
	{
		if(mArCoefficients != NULL)
		{
			delete [] mArCoefficients;
			mArCoefficients = NULL;
		}
		if(mArMatrix != NULL)
		{
			for(int i = 0; i < mArDegree; ++i)
			{
				delete [] mArMatrix[i];
			}
			delete [] mArMatrix;
			mArMatrix = NULL;
		}
	}
}

void ViArmaNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	(this->*calculateNoisePointer)(samples);
}

void ViArmaNoiseDetector::calculateNoiseNative(QQueue<qreal> &samples)
{
	static bool failedAR, failedMA;
	static int i;
	static qreal theMean, theVariance, prediction;

	while(samples.size() > mWindowSize)
	{
		prediction = 0;
		failedAR = failedMA = false;

		if(mType == MA || mType == ARMA)
		{
			theMean = mean(samples);
			theVariance = variance(samples, theMean);

			for(i = 0; i < mWindowSize; ++i)
			{
				mWindowData[i] = generateNoise(theVariance);
			}

			if(leastSquareFit(mWindowData, mMaDegree, mMaCoefficients, mMaMatrix))
			{
				//prediction += generateNoise(variance); // This extra error term slightly reduces the accuracy
				for(i = 0; i < mMaDegree; ++i)
				{
					prediction += mMaCoefficients[i] * mWindowData[mWindowSize - 1 - i];
				}
			}
			else failedMA = true;
		}

		if(mType == AR || mType == ARMA)
		{
			for(i = 0; i < mWindowSize; ++i)
			{
				mWindowData[i] = samples[i];
			}

			if(leastSquareFit(mWindowData, mArDegree, mArCoefficients, mArMatrix))
			{
				for(i = 0; i < mArDegree; ++i)
				{
					prediction += (mArCoefficients[i] * mWindowData[mWindowSize - 1 - i]);
				}
			}
			else failedAR = true;

		}
		if(failedAR && failedMA) setNoise(0);
		else setNoise(qAbs(samples[mWindowSize] - prediction));
		samples.removeFirst();
	}
}

void ViArmaNoiseDetector::calculateNoiseGretl(QQueue<qreal> &samples)
{
	static bool failed;
	static int i, j, error, end, max;

	max = mWindowSize + PREDICTION_SIZE - 1;
	while(samples.size() > max)
	{
		for(i = 0; i < PREDICTION_SIZE; ++i) mGretlPredictions[i] = 0;
		failed = false;

		//double samples2[] = {0.386608, 0.366039, 0.286813, 0.200995, 0.170232, 0.214087, 0.2978, 0.362377, 0.370678, 0.335734, 0.299326, 0.279122, 0.258583, 0.216163, 0.166723, 0.147862, 0.181066, 0.250587, 0.32197, 0.374767, 0.409986, 0.433576, 0.441389, 0.428694, 0.409192, 0.399274, 0.392956, 0.367473, 0.322581, 0.279763, 0.244026, 0.189215, 0.103, 0.0160833, -0.0278329, -0.0199896, 0.0155339, 0.0588702, 0.112308, 0.184637, 0.260445, 0.309488, 0.31489, 0.293893, 0.272011, 0.261513, 0.266305, 0.28898, 0.314646, 0.314493, 0.288827, 0.274178, 0.288491, 0.298624, 0.271126, 0.224067, 0.193304, 0.178838, 0.156438, 0.123417, 0.100681, 0.0996429, 0.108707, 0.106967, 0.0872219, 0.0608234, 0.0429701, 0.030549, 0.0176397, 0.0219733, 0.0670186, 0.13715, 0.189032, 0.206702, 0.212195, 0.211554, 0.182287, 0.117832, 0.0427564, -0.0155034, -0.0448927, -0.0307016, 0.0241096, 0.0831019, 0.100955, 0.078341, 0.0525834, 0.0441603, 0.0390332, 0.018891, -0.00869778, -0.0246895, -0.0259713, -0.0302744, -0.0562151, -0.0856655, -0.0904263, -0.0758385, -0.0914029, -0.156743, -0.225593, -0.242531, -0.20896, -0.167699, -0.139775, -0.126499, -0.133061, -0.168096, -0.212317, -0.230537, -0.208014, -0.16187, -0.122684, -0.11359, -0.137486, -0.172857, -0.192297, -0.194159, -0.202368, -0.233161, -0.273415, -0.297647, -0.297342, -0.281075, -0.265908, -0.269051, -0.294961, -0.329417, -0.346141, -0.334849, -0.299905, -0.247078, -0.182226, -0.125217, -0.10007, -0.109867, -0.138951, -0.182379, -0.248146, -0.322672, -0.364391, -0.344737, -0.286264, -0.238655, -0.229438, -0.247475, -0.273751, -0.307108, -0.354076, -0.397717, -0.400372, -0.344798, -0.264687, -0.213691, -0.213721, -0.240394, -0.263588, -0.2772, -0.296854, -0.332011, -0.382733, -0.439375, -0.495315, -0.536821, -0.550462, -0.530778, -0.484603, -0.419752, -0.334422, -0.239631, -0.1742, -0.172338, -0.221961, -0.277535, -0.31428, -0.338908, -0.351939, -0.341472, -0.30961, -0.280801, -0.263375, -0.249184, -0.233802, -0.224952, -0.217505, -0.196997, -0.166814, -0.149785, -0.157262, -0.179357, -0.198187, -0.201666, -0.187597, -0.170721, -0.166814, -0.175481, -0.177709, -0.167425, -0.156407, -0.148137, -0.130802, -0.101657, -0.0776086, -0.0685446, -0.0640584, -0.056093, -0.0572832, -0.0818201, -0.121067, -0.15479, -0.166478, -0.153294, -0.111515, -0.0488296, 0.0104068, 0.0348216, 0.0221564, 0.00585955, 0.0104984, 0.0151067, -0.0198981, -0.0835902, -0.123905, -0.116764, -0.0988189, -0.102298, -0.103397, -0.0652791, 0.00354015, 0.0665304, 0.108036, 0.125462, 0.0994903, 0.027192, -0.0451064, -0.0601825, -0.025483, 0.0052797, 0.0085757, 0.0102237, 0.0251167, 0.0340281, 0.0261849, 0.0216681, 0.033784, 0.046144, 0.0451369, 0.0480972, 0.0710166, 0.0985137, 0.108402, 0.109073, 0.125706, 0.152501, 0.157598, 0.129276, 0.0952788, 0.0854823, 0.100681, 0.12534, 0.15659, 0.19248, 0.215796, 0.209571, 0.181341, 0.157201, 0.140233, 0.109561, 0.0578631, 0.0169988, 0.0236824, 0.0720237, 0.113956, 0.118168, 0.103, 0.107761, 0.143376, 0.184973, 0.20896, 0.211066, 0.196783, 0.174932, 0.163305, 0.175268, 0.200842, 0.216437, 0.217109, 0.21775, 0.216437, 0.199896, 0.171178, 0.155309, 0.158757, 0.162969, 0.164373, 0.181768, 0.214972, 0.234016, 0.22837, 0.227058, 0.245338, 0.247383, 0.200659, 0.134098, 0.107303, 0.135228, 0.183905, 0.225074, 0.263039, 0.302347, 0.325571, 0.315317, 0.280526, 0.241157, 0.20661, 0.170843, 0.137333, 0.124088, 0.140172, 0.174078, 0.20777, 0.233497, 0.248817, 0.240638, 0.200568, 0.144475, 0.105899, 0.108798, 0.153569, 0.222266, 0.283975, 0.311167, 0.295236, 0.250496, 0.186163, 0.105655, 0.0183721, -0.0534684, -0.101108, -0.133427, -0.148045, -0.124027, -0.0541398, 0.0390027, 0.132328, 0.220008, 0.292703, 0.31724, 0.273782, 0.18659, 0.10538, 0.0619831, 0.0573443, 0.0777612, 0.104007, 0.110446, 0.0809656, 0.029603, -0.011536, -0.018067, 0.00357067, 0.0293588, 0.0334178, 0.00720237, -0.0280465, -0.031373, 0.0201727, 0.0977203, 0.145268, 0.135411, 0.0876797, 0.034547, -0.00668355, -0.0260018, -0.0138554, 0.0332957, 0.1077, 0.19599, 0.271523, 0.288919, 0.223121, 0.101932, -0.0126347, -0.089114, -0.132176, -0.143254, -0.10773, -0.0298471, 0.0448012, 0.0730003, 0.0477615, 0.00567644, -0.0143742, 0.00546281, 0.047029, 0.0797754, 0.0867031, 0.0768761, 0.0555742, 0.0160833, -0.0457778, -0.105289, -0.123875, -0.0879849, -0.0177007, 0.0476394, 0.0770898, 0.0619221, 0.0179754, -0.0236824, -0.0353099, -0.00897244, 0.0464492, 0.108585, 0.145299, 0.127171, 0.0561541, -0.0278329, -0.0873135, -0.121586, -0.152532, -0.174078, -0.155705, -0.0916776, -0.018067, 0.0252388, 0.0351268, 0.0275582, 0.0158696, 0.00927763, 0.0142827, 0.0174871, -0.00720237, -0.0582598, -0.0906095, -0.0607624, 0.0184027, 0.0936613, 0.133244, 0.144597, 0.14243, 0.11774, 0.0616474, -0.0135807, -0.0795312, -0.125889, -0.154332, -0.163549, -0.147221, -0.115452, -0.0901517, -0.0814234, -0.0785241, -0.0696432, -0.0561541, -0.0441298, -0.0407117, -0.0537736, -0.0876797, -0.125828, -0.148259, -0.147679, -0.136937, -0.129154, -0.138737, -0.180334, -0.247902, -0.295602, -0.28135, -0.217261, -0.153996, -0.112918, -0.0744652, -0.0291757, -0.000518815, -0.0019837, -0.0121158, -0.010712, -0.00769066, -0.0151982, -0.029603, -0.0465712, -0.0808741, -0.135319, -0.188238, -0.217231, -0.226173, -0.221992, -0.195959, -0.1442, -0.0895413, -0.0606098, -0.0613117, -0.075869, -0.0966826, -0.115421, -0.113132, -0.0703757, -0.00207526, 0.0459609, 0.0437941, 0.00909452, -0.012299, 0.000183111, 0.0174871, -0.0028077, -0.0686666, -0.142949, -0.185553, -0.187628, -0.165654, -0.130985, -0.94998, -0.954985, -0.95999, -0.964995, -0.0722068, -0.093112, -0.10596, -0.107273, -0.101291, -0.104404, -0.137364, -0.178991, -0.180212, -0.12183, -0.0549638, -0.0464492, -0.108402, -0.20014, -0.269417, -0.284555, -0.235633, -0.155034, -0.104129, -0.114505, -0.150639, -0.157567, -0.127293, -0.0906705, -0.0630512, -0.046968, -0.062624, -0.135472, -0.239753, -0.305857, -0.293405, -0.231819, -0.17365, -0.136937, -0.108524, -0.0843837, -0.0744041, -0.076632, -0.0714133, -0.0504776, -0.0301828, -0.0261238, -0.0284433, -0.0149846, 0.0184027, 0.0522172, 0.0702231, 0.0674154, 0.0492264, 0.025544, 0.00708029, -0.0101627, -0.0457472, -0.10535, -0.15244, -0.146672, -0.0955535, -0.0569475, -0.0681784, -0.110141, -0.139439, -0.140599, -0.130467, -0.123325, -0.112888, -0.0796838, -0.0207221, 0.0441908, 0.0834376, 0.0910978, 0.0823695, 0.0674459, 0.0279244, -0.0489822, -0.142125, -0.205634, -0.210608, -0.160222, -0.0807215, -0.009064, 0.0274361, 0.0283517, 0.016541, 0.00350963, -0.0243538, -0.0729087, -0.122105, -0.144353, -0.136937, -0.110996, -0.0736106, -0.0339366, -0.014008, -0.0309763, -0.0680868, -0.0860927, -0.0671407, -0.033723, -0.025544, -0.0539872, -0.0922269, -0.103214, -0.0723899, -0.0187689, 0.0309763, 0.0572222, 0.0485855, 0.00100711, -0.0738243, -0.136662, -0.145909, -0.0893887, 0.00625629, 0.101444, 0.168035, 0.192694, 0.173193, 0.131474, 0.105289, 0.112247, 0.118564, 0.0750755, -0.0193793, -0.113376, -0.157567, -0.150426, -0.112156, -0.0579546, -0.000579852, 0.0361339, 0.0400708, 0.0349132, 0.0559099, 0.113071, 0.174993, 0.202979, 0.183935, 0.141301, 0.116977, 0.134678, 0.180792, 0.220222, 0.229072, 0.204474, 0.15595, 0.0971709, 0.0513932, 0.0344554, 0.0416578, 0.0533464, 0.0617389, 0.0748314, 0.0985137, 0.118717, 0.119144, 0.0944243, 0.0587176, 0.0326243, 0.0292367, 0.0463881, 0.0781579, 0.127293, 0.201086, 0.28843, 0.352458, 0.356853, 0.298257, 0.207648, 0.131077, 0.103061, 0.129948, 0.189978, 0.241401, 0.252937, 0.221473, 0.171361, 0.133518, 0.120243, 0.126926, 0.148198, 0.189398, 0.247536, 0.296152, 0.304361, 0.269784, 0.217719, 0.163976, 0.103336, 0.0382397, -0.00643941, -0.00488296, 0.0388501, 0.103458, 0.172948, 0.234657, 0.271096, 0.269112, 0.245582, 0.235725, 0.258126, 0.291818, 0.300424, 0.265633, 0.202826, 0.134373, 0.0740989, 0.0274972, 0.00598163, 0.0176702, 0.0484634, 0.0678732, 0.0569781, 0.029725, 0.0106815, 0.0126347, 0.0241401, 0.0344554, 0.0437635, 0.0645772, 0.101016, 0.134892, 0.138524, 0.105472, 0.0603961, 0.0350963, 0.0339366, 0.0414441, 0.0503555, 0.0680868, 0.0910672, 0.0970183, 0.0707114, 0.0265206, -0.00592059, -0.0151677, -0.00671407, 0.0154118, 0.0493179, 0.0800806, 0.0834376, 0.0568255, 0.0224006, 0.000824, -0.0117496, -0.0208747, -0.0253914, -0.0274056, -0.0415662, -0.0664693, -0.0787378, -0.070101, -0.0690023, -0.100192, -0.135807, -0.128605, -0.0779138, -0.031312, -0.0233772, -0.0347301, -0.0327158, -0.0127873, 0.0029603, 0.00634785, 0.0119022, 0.0372936, 0.0743126, 0.0956755, 0.0776696, 0.0204169, -0.0552385, -0.122135, -0.159062, -0.1583, -0.124699, -0.0719932, -0.0145878, 0.027131, 0.0326853, 0.000122074, -0.0444655, -0.0677816, -0.066805, -0.0619221, -0.0653706, -0.0648213, -0.0528275, -0.0397961, -0.0409253, -0.0553606, -0.0752281, -0.0944853, -0.108219, -0.110904, -0.108676, -0.112888, -0.117405, -0.098178, -0.0513627, -0.0130619, -0.0202033, -0.0592975, -0.0834376, -0.0754723, -0.0690023, -0.098117, -0.151616, -0.188696, -0.192572, -0.175726, -0.153905, -0.124302, -0.0885342, -0.061861, -0.059389, -0.0732749, -0.0823389, -0.0785852, -0.0736412, -0.0819117, -0.10065, -0.111393, -0.089938, -0.0307627, 0.0390942, 0.0748314, 0.0541398, 0.00607318, -0.0206, -0.00979644, 0.0100711, 0.0104678, -0.00784326, -0.0316172, -0.0661336, -0.118381, -0.168188, -0.175665, -0.129368, -0.0628376, -0.0208136, -0.0175787, -0.0371105, -0.0586261, -0.065096, -0.0408948, 0.0218207, 0.104495, 0.165075, 0.1713, 0.126225, 0.0628986, 0.0166326, 0.010651, 0.0474563, 0.113987, 0.177374, 0.205878, 0.18424, 0.123447, 0.0472427, -0.0163274, -0.0480056, -0.0455031, -0.0307932, -0.0254219, -0.025483, -0.00537126, 0.041963, 0.0870388, 0.0984527, 0.0838649, 0.0777306, 0.0994903, 0.131687, 0.152135, 0.152562, 0.138249, 0.111179, 0.0791345, 0.0528581, 0.040315, 0.037141, 0.0458693, 0.0804468, 0.143529, 0.20069, 0.204169, 0.149907, 0.0880154, 0.0730003, 0.101596, 0.129643, 0.124058, 0.0991852, 0.0823695, 0.0798364, 0.0766015, 0.0627766, 0.0483413, 0.0390332, 0.0374462, 0.0487381, 0.0937834, 0.177465, 0.262215, 0.283547, 0.214393, 0.0982696, 0.00927763, -0.0200507, -0.010773, 0.00347911, 0.0209357, 0.0488907, 0.0775781, 0.0881985, 0.0867641, 0.104038, 0.153966, 0.209479, 0.229041, 0.202002, 0.156865, 0.12363, 0.0985443, 0.0591144, 0.000488296, -0.04944, -0.0705893, -0.0731529, -0.0859401, -0.108554, -0.114109, -0.0788598, -0.0137333, 0.0538041, 0.10303, 0.134129, 0.152379, 0.158086, 0.145116, 0.115482, 0.084933, 0.0702231, 0.0707419, 0.0684225, 0.0496231, 0.0164495, -0.023072, -0.0636006, -0.100345, -0.12006, -0.111576, -0.0821863, -0.0528886, -0.0414441, -0.0464797, -0.0575274, -0.0694906, -0.0844752, -0.102054, -0.110782, -0.0999481, -0.0632038, -0.00170904, 0.0713218, 0.128239, 0.139134, 0.106876, 0.0641804, 0.0341502, -0.00332652, -0.0742821, -0.157506, -0.190252, -0.141118, -0.0450758, 0.0321055, 0.0539567, 0.0222785, -0.0487991, -0.132481, -0.185308, -0.163152, -0.0700705, 0.035432, 0.083285, 0.0493179, -0.0348216, -0.119724, -0.171148, -0.185705, -0.174566, -0.151097, -0.12122, -0.0844447, -0.0403455, 0.000854518, 0.0234382, 0.0202948, 0.00289926, -0.00814844, -0.00915555, -0.0113834, -0.0281381, -0.0613422, -0.10596, -0.157689, -0.201636, -0.221198, -0.210334, -0.188849, -0.184668, -0.208808, -0.242347, -0.255135, -0.232337, -0.188971, -0.153661, -0.141575, -0.144047, -0.148564, -0.159337, -0.193548, -0.255409, -0.321177, -0.351054, -0.323313, -0.254372, -0.183874, -0.142766, -0.135563, -0.149876, -0.172826, -0.196234, -0.222388, -0.260933, -0.314341, -0.359355, -0.362285, -0.317331, -0.262276, -0.246101, -0.284433, -0.351024, -0.407575, -0.432661, -0.425733, -0.39668, -0.356334, -0.321696, -0.312998, -0.339305, -0.380108, -0.399121, -0.374187, -0.323283, -0.284433, -0.275063, -0.280679, -0.281655, -0.28428, -0.298837, -0.309183, -0.286081, -0.236579, -0.209204, -0.235908, -0.290445, -0.325968, -0.325388, -0.300821, -0.262276, -0.210547, -0.165197, -0.152593, -0.17481, -0.202216, -0.212683, -0.219916, -0.241371, -0.262947, -0.248543, -0.191809, -0.123356, -0.0701926, -0.031312, -0.0106815, -0.0275887, -0.0830103, -0.133549, -0.141575, -0.122623, -0.11948, -0.12891, -0.112674, -0.0611896, -0.0155034, -0.0119633, -0.035432, -0.0456252, -0.0344859, -0.0269173, -0.0401624, -0.0546281, -0.0429701, -0.00744652, 0.0212409, 0.025544, 0.0177313, 0.0142521, 0.0119633, -0.000366222, -0.014771, -0.0165105, -0.00759911, -0.0156255, -0.061037, -0.127903, -0.168493, -0.145238, -0.0685446, 0.010712, 0.0523087, 0.0591144, 0.0575274, 0.0493484, 0.0183111, -0.0317392, -0.0573748, -0.021424, 0.0553606, 0.107242, 0.0921659, 0.0323496, -0.0177618, -0.0387585, -0.050264, -0.0691855, -0.0746483, -0.0428175, 0.0159307, 0.0681784, 0.0975372, 0.109439, 0.106052, 0.0794397, 0.0408948, 0.0224921, 0.0465712, 0.10007, 0.155492, 0.198218, 0.223365, 0.219489, 0.1807, 0.125858, 0.0874355, 0.0818812, 0.0973235, 0.115604, 0.125828, 0.120609, 0.09888, 0.0726951, 0.0612812, 0.0686666, 0.0849635, 0.103946, 0.135228, 0.177862, 0.217353, 0.239296, 0.24839, 0.251381, 0.249611, 0.248543, 0.26664, 0.306009, 0.340861, 0.33787, 0.296457, 0.243416, 0.203742, 0.188513, 0.200201, 0.237129, 0.286691, 0.328288, 0.34788, 0.346385, 0.335826, 0.329386, 0.331553, 0.338511, 0.34431, 0.346782, 0.340556, 0.319437, 0.279305, 0.238289, 0.224677, 0.255287, 0.306955, 0.336436, 0.327525, 0.314005, 0.330546, 0.357646, 0.345714, 0.285684, 0.229011, 0.221625, 0.255562, 0.295633, 0.318339, 0.317667, 0.299692, 0.281808, 0.285958, 0.309488, 0.333872, 0.352519, 0.375805, 0.40144, 0.408979, 0.382519, 0.326365, 0.249519, 0.16306, 0.079928, 0.0185553, -0.0143437, -0.023896, -0.0245674, -0.0228278, -0.002472, 0.0599078, 0.163884, 0.266976, 0.32664, 0.339579, 0.322916, 0.286294, 0.232276, 0.182134, 0.159093, 0.165838, 0.185461, 0.196875, 0.179937, 0.127384, 0.0614338, 0.0218513, 0.0245064, 0.046968, 0.0596026, 0.0456862, 0.00421155, -0.0411084, -0.0471206, 0.00543229, 0.072512, 0.0842006, 0.0215766, -0.0660115, -0.11652, -0.103946, -0.0435804, 0.0350047, 0.106723, 0.160283, 0.180609, 0.149144, 0.066805, -0.0211493, -0.0689718, -0.0732444, -0.0691549, -0.0723289, -0.0745567, -0.0715659, -0.0765709, -0.0950041, -0.119663, -0.137822, -0.132878, -0.0906095, -0.0200507, 0.0432142, 0.0653401, 0.0414441, -0.0157781, -0.0795312, -0.119755, -0.119785, -0.101535, -0.0940275, -0.0910978, -0.0618305, -0.00134281, 0.0670797, 0.122349, 0.156468, 0.155095, 0.108158, 0.0349742, -0.031373, -0.0769066, -0.110569, -0.139317, -0.164892, -0.191168, -0.211432, -0.204779, -0.160497, -0.104526, -0.0783715, -0.0901822, -0.106021, -0.0887478, -0.0439467, -0.0130924, -0.024659, -0.0678121, -0.112674, -0.14597, -0.166906, -0.171819, -0.163488, -0.156529, -0.152562, -0.124149, -0.0554827, 0.0206915, 0.0613422, 0.0616779, 0.0411695, -0.00244148, -0.0797143, -0.163213, -0.206275, -0.197913, -0.170415, -0.151799, -0.140019, -0.128605, -0.12418, -0.136204, -0.162542, -0.189886, -0.20423, -0.19895, -0.180822, -0.167028, -0.166784, -0.17307, -0.174078, -0.172643, -0.185583, -0.216956, -0.240883, -0.221351, -0.153691, -0.0732139, -0.0203253, -0.00646992, -0.0126652, -0.0160222, -0.0102542, -0.00912503, -0.0351573, -0.0910062, -0.146794, -0.166295, -0.138432, -0.082522, -0.0242622, 0.0161443, 0.0267647, 0.0113529, -0.00946074, -0.0170598, -0.00805689, 0.0139775, 0.0446486, 0.0695212, 0.0679342, 0.0426649, 0.027955, 0.0555742, 0.11301, 0.161138, 0.174566, 0.163366, 0.147679, 0.136174, 0.12421, 0.111087, 0.0999176, 0.0879543, 0.0720237, 0.0624104, 0.0747703, 0.0981475, 0.101321, 0.0746178, 0.0550249, 0.0750755, 0.12064, 0.148961, 0.142125, 0.11127, 0.0719321, 0.0344554, 0.0174261, 0.0390942, 0.0969268, 0.160009, 0.185827, 0.151097, 0.0712912, -0.00460829, -0.034669, -0.0144047, 0.0363475, 0.100772, 0.169774, 0.224311, 0.24546, 0.231452, 0.200873, 0.169012, 0.144444, 0.128727, 0.118748, 0.104312, 0.0777612, 0.0489517, 0.0357982, 0.0455947, 0.0662557, 0.0774865, 0.0743126, 0.0647908, 0.0600604, 0.0560015, 0.0441908, 0.0296335, 0.0260933, 0.0407727, 0.0620746, 0.0786462, 0.0938139, 0.118442, 0.152196, 0.178777, 0.187078, 0.178625, 0.16718, 0.155614, 0.140477, 0.119907, 0.102054, 0.09653, 0.100497, 0.0941496, 0.0548418, -0.0234993, -0.111637, -0.164953, -0.161504, -0.1189, -0.0729087, -0.0402844, -0.0173345, 0.00607318, 0.0351573, 0.0685141, 0.0922575, 0.0856655, 0.0423902, -0.0105594, -0.0333872, -0.00500504, 0.0512711, 0.093234, 0.0921964, 0.0588092, 0.0220649, 0.00323496, -0.00177007, -0.00271615, -0.00613422, -0.0143437, -0.0234993, -0.0240486, -0.0116886, 0.00225837, -0.00372326, -0.0331736, -0.0738243, -0.106693, -0.115329, -0.0926542, -0.0487991, -0.0115665, 0.00613422, 0.0138249, 0.0209662, 0.0139775, -0.014893, -0.0476394, -0.0611286, -0.0630818, -0.0788598, -0.114872, -0.153783, -0.173864, -0.165349, -0.94998, -0.954985, -0.95999, -0.964995, -0.0385449, -0.0297555, -0.0143132, -0.00186163, -0.0101016, -0.0434278, -0.0864589, -0.118351, -0.146611, -0.201666, -0.285775, -0.355632, -0.365673, -0.31547, -0.240669, -0.172552, -0.121464, -0.0831629, -0.0485244, -0.00738548, 0.0395215, 0.084048, 0.11185, 0.109043, 0.071749, 0.019715, -0.0168767, -0.0286569, -0.0318308, -0.042085, -0.0602741, -0.0886258, -0.127262, -0.158788, -0.163244, -0.14774, -0.1348, -0.120212, -0.0839869, -0.0343638, -0.00936918, -0.023957, -0.0558794, -0.0750755, -0.0667135, -0.028779, 0.0239265, 0.0637226, 0.0777917, 0.0805689, 0.0797754, 0.0592975, 0.0132145, -0.0274667, -0.0377514, -0.0305795, -0.038728, -0.0612812, -0.0753502, -0.0656453, -0.0444349, -0.0332957, -0.0432752, -0.0628071, -0.0695212, -0.0541093, -0.0243843, 0.0177923, 0.0730918, 0.123325, 0.131382, 0.0836512, 0.00796533, -0.0644856, -0.122044, -0.162969, -0.17542, -0.151952, -0.0970794, -0.0282907, 0.0342723, 0.0732749, 0.0901517, 0.100101, 0.108066, 0.103549, 0.0843532, 0.0619831, 0.0361339, -0.00616474, -0.0618915, -0.105441, -0.118534, -0.100528, -0.0531632, 0.0149541, 0.0710166, 0.0809961, 0.0466628, -0.00531022, -0.0610981, -0.112491, -0.131413, -0.0929899, -0.0215155, 0.0259102, 0.0218207, -0.0144658, -0.0518509, -0.0731834, -0.0657674, -0.024781, 0.0373241, 0.0932035, 0.128452, 0.142003, 0.131718, 0.0863063, 0.00793481, -0.0752281, -0.124668, -0.124973, -0.0918607, -0.0495926, -0.0136723, 0.00442518, -0.00781274, -0.0496231, -0.0961943, -0.125401, -0.135197, -0.13834, -0.14182, -0.151006, -0.169012, -0.181646, -0.16361, -0.110813, -0.0564898, -0.0364391, -0.0485855, -0.072573, -0.0897855, -0.0889004, -0.0560625, 0.002472, 0.0515152, 0.0535905, 0.00836207, -0.0507523, -0.0910978, -0.102512, -0.092349, -0.0702536, -0.0409558, -0.010773, 0.0105289, 0.0103153, -0.0109256, -0.0383007, -0.0516678, -0.0390942, -0.00671407, 0.0316172, 0.0606708, 0.0761132, 0.076632, 0.0613422, 0.0347606, 0.0126957, 0.0141606, 0.0387585, 0.0719626, 0.0976592, 0.107761, 0.093997, 0.0577105, 0.0162969, -0.009064, -0.0127873, -0.00381481, 0.0085757, 0.0115055, -0.0104984, -0.0533769, -0.087405, -0.0873135, -0.066805, -0.0553911, -0.0599689, -0.060976, -0.0431837, -0.0157781, 0.00231941, -0.00390637, -0.0337535, -0.0728172, -0.104923, -0.122806, -0.127384, -0.124973, -0.128666, -0.150731, -0.178777, -0.178198, -0.130192, -0.0581072, -0.00137333, 0.0250862, 0.037019, 0.0499283, 0.0627766, 0.0534684, 0.00183111, -0.0867946, -0.169561, -0.202399, -0.178838, -0.128819, -0.0845973, -0.06592, -0.0773949, -0.107517, -0.121433, -0.0988495, -0.0540483, -0.0219733, -0.0158391, -0.019837, -0.0186773, -0.0106815, -0.00650044, -0.0102847, -0.0163274, -0.00970489, 0.0112918, 0.0307321, 0.0301828, 0.0142521, 0.00476089, 0.013123, 0.028016, 0.024781, -0.00860622, -0.0567034, -0.0833766, -0.0698263, -0.0402844, -0.0339061, -0.0600299, -0.0945769, -0.115024, -0.114353, -0.0836512, -0.0198065, 0.0553301, 0.105991, 0.122745, 0.11887, 0.0998871, 0.0563066, -0.00265511, -0.0479446, -0.0622883, -0.0545976, -0.0364086, -0.00872829, 0.0237129, 0.0444349, 0.0466323, 0.0384838, 0.033845, 0.0337535, 0.0383312, 0.0506302, 0.0704062, 0.0869778, 0.0860622, 0.0687582, 0.0490738, 0.0386059, 0.0348216, 0.0292062, 0.022248, 0.0182501, 0.0146794, 0.00479141, -0.0110172, -0.0187994, -0.00759911, 0.0256966, 0.0724815, 0.106052, 0.0971099, 0.041261, -0.020539, -0.0433668, -0.0258797, -0.00424207, -0.0013123, -0.00292978, 0.0100711, 0.0415662, 0.0787683, 0.109165, 0.120792, 0.108768, 0.0845363, 0.0607929, 0.0406812, 0.0167852, -0.009888, -0.0451979, -0.101627, -0.181951, -0.256203, -0.282662, -0.255074, -0.196997, -0.131993, -0.0625629, 0.00564592, 0.0583514, 0.0868862, 0.0944548, 0.0833766, 0.0514542, 0.011475, -0.0116581, -0.00830103, -0.000640889, -0.0145268, -0.04532, -0.07004, -0.0763878, -0.0781884, -0.0883816, -0.102786, -0.105472, -0.0922575, -0.0737938, -0.0674764, -0.0786157, -0.0986663, -0.111148, -0.109317, -0.100864, -0.0993988, -0.101871, -0.0899075, -0.0525224, -0.00613422, 0.0220954, 0.0222785, 0.00668355, -0.0162969, -0.0425733, -0.0641499, -0.0658589, -0.0455031, -0.0164495, 0.00949126, 0.0267647, 0.026368, 0.000366222, -0.0401624, -0.073336, -0.0891751, -0.0901822, -0.0720237, -0.0258492, 0.0308237, 0.0657369, 0.0632038, 0.0374767, 0.00335704, -0.0345775, -0.0665914, -0.0744041, -0.056856, -0.0365612, -0.0269784, -0.0129704, 0.0306101, 0.0984527, 0.156407, 0.17478, 0.157598, 0.1283, 0.0968352, 0.0545671, -0.000762963, -0.0507828, -0.0823695, -0.102634, -0.122166, -0.128513, -0.101474, -0.0456557, 0.00824, 0.0314341, 0.0211798, -0.00479141, -0.0226142, -0.0210578, -0.00479141, 0.0143437, 0.0307627, 0.0412305, 0.0346995, 0.000335704, -0.0564287, -0.106418, -0.120212, -0.0895413, -0.0322275, 0.0217597, 0.0518204, 0.0541093, 0.0334788, -0.00234993, -0.0401318, -0.0647908, -0.0687277, -0.061037, -0.0476699, -0.0264901, -0.00119022, 0.00747703, -0.0163274, -0.0579241, -0.0884732, -0.0970183, -0.100192, -0.113651, -0.135716, -0.154271, -0.163701, -0.169836, -0.17481, -0.168676, -0.142277, -0.101291, -0.0679952, -0.0511795, -0.037843, -0.0143742, 0.0117801, 0.0213935, 0.009064, -0.0137944, -0.0350047, -0.0509659, -0.0619221, -0.0685446, -0.0708029, -0.0679952, -0.0524918, -0.0240486, 0.0120853, 0.0376904, 0.0456557, 0.0372936, 0.0261544, 0.0212714, 0.0266427, 0.0404981, 0.0588092, 0.0796533, 0.103275, 0.131352, 0.155004, 0.157262, 0.134709, 0.111606, 0.118809, 0.150334, 0.170415, 0.162572, 0.152104, 0.163305, 0.178533, 0.164953, 0.127567, 0.102481, 0.105075, 0.107944, 0.0852687, 0.047792, 0.0287484, 0.046144, 0.0869167, 0.130192, 0.159032, 0.172369, 0.175756, 0.17893, 0.184118, 0.187506, 0.181036, 0.162206, 0.132145, 0.0912503, 0.0469375, 0.0158391, 0.0225532, 0.0687887, 0.129032};

		for(i = 0; i < mWindowSize; ++i)
		{
			mGretlData->Z[1][i] = samples[i];
			//cout<<samples[i]<<", ";
		}
		//cout<<endl;

		MODEL *model = gretl_model_new();
		(this->*gretlModelPointer)(model);
		/*mGretlParameters[1]=20;
		mGretlParameters[3]=0;
*model = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);*/
		if(model->errcode) failed = true;
		else
		{
			FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
			if(error) failed = true;
			else
			{
				if(forecast->nobs == mWindowSize + PREDICTION_SIZE)
				{
					end = max + 1;
					for(i = mWindowSize, j = 0; i < end; ++i, ++j) mGretlPredictions[j] = forecast->fitted[i];
				}
				free_fit_resid(forecast);
			}
		}
		gretl_model_free(model);

		if(failed)
		{
			for(i = 0; i < PREDICTION_SIZE; ++i) setNoise(0);
		}
		else
		{
			for(i = 0; i < PREDICTION_SIZE; ++i) setNoise(qAbs(samples[mWindowSize + i] - mGretlPredictions[i]));
		}
		for(i = 0; i < PREDICTION_SIZE; ++i) samples.removeFirst();
	}
}

void ViArmaNoiseDetector::gretlFixedModel(MODEL *model)
{
	mGretlParameters[1] = mArDegree;
	mGretlParameters[3] = mMaDegree;
	while(true)
	{
		*model = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);
		if(model->errcode)
		{
			if(mGretlParameters[3] > 0)
			{
				--mGretlParameters[3];
			}
			else if(mGretlParameters[1] > 0)
			{
				mGretlParameters[3] = mMaDegree;
				--mGretlParameters[1];
			}
		}
		else break;
	}
}

void ViArmaNoiseDetector::gretlAutocorrelationModel(MODEL *model)
{
	static int error, i, arDegree, maDegree;

	error = 0; // We have to set this manually, because gretl doesn't
	gretl_matrix *matrix = acf_matrix(mGretlData->Z[1], MAXIMUM_TEST_DEGREE, NULL, mWindowSize, &error);

	if(error)
	{
		gretlFixedModel(model);
		return;
	}

	maDegree = 0;
	for(i = 0; i < matrix->rows; ++i)
	{
		if(abs(gretl_matrix_get(matrix, i, 0)) < mConfidenceLevel)
		{
			maDegree = i;
			break;
		}
	}

	arDegree = 0;
	for(i = 0; i < matrix->rows; ++i)
	{
		if(abs(gretl_matrix_get(matrix, i, 1)) < mConfidenceLevel)
		{
			arDegree = i;
			break;
		}
	}

	gretl_matrix_free(matrix);
	mGretlParameters[1] = arDegree;
	mGretlParameters[3] = maDegree;
	while(true)
	{
		*model = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);
		if(model->errcode)
		{
			if(mGretlParameters[3] > 0)
			{
				--mGretlParameters[3];
			}
			else if(mGretlParameters[1] > 0)
			{
				mGretlParameters[3] = maDegree;
				--mGretlParameters[1];
			}
		}
		else break;
	}
}

void ViArmaNoiseDetector::gretlBestModel(MODEL *model)
{
	static qreal current, min;
	static int arOrder, maOrder, arOrderBest, maOrderBest;
	static MODEL *tempModel;

	arOrder = 0, maOrder = 0;
	arOrderBest = 0, maOrderBest = 0;

	min = DBL_MAX;
	mGretlParameters[3] = 0;
	while(true)
	{
		mGretlParameters[1] = arOrder;
		tempModel = gretl_model_new();
		//cout<<mGretlParameters[1]<<"\t"<<mGretlParameters[3]<<endl;
		*tempModel = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);
		current = (this->*gretlOrderPointer)(tempModel);
		gretl_model_free(tempModel);
		if(current > min)
		{
			arOrderBest = arOrder - 1;
			break;
		}
		min = current;
		++arOrder;
		if(arOrder > MAXIMUM_TEST_DEGREE) break;
	}

	min = DBL_MAX;
	mGretlParameters[1] = arOrderBest;
	while(true)
	{
		mGretlParameters[3] = maOrder;
		tempModel = gretl_model_new();
		*tempModel = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);
		current = (this->*gretlOrderPointer)(tempModel);
		gretl_model_free(tempModel);
		if(current > min)
		{
			maOrderBest = maOrder - 1;
			break;
		}
		min = current;
		++maOrder;
		if(maOrder > MAXIMUM_TEST_DEGREE) break;
	}

	if(arOrderBest < 0) arOrderBest = 0;
	if(maOrderBest < 0) maOrderBest = 0;
	mGretlParameters[1] = arOrderBest;
	mGretlParameters[3] = maOrderBest;
	static qint64 a=0, m=0, c=0;
	++c;
	a+=arOrderBest;
	m+=maOrderBest;
	cout<<a/c<<"\t"<<m/c<<endl;
	*model = arma_model(mGretlParameters, NULL, mGretlData, (gretlopt) mGretlEstimation, NULL);
}

qreal ViArmaNoiseDetector::gretlR2(MODEL *model)
{
	return model->rsq;
}

qreal ViArmaNoiseDetector::gretlAR2(MODEL *model)
{
	return model->adjrsq;
}

qreal ViArmaNoiseDetector::gretlAIC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff;
}

qreal ViArmaNoiseDetector::gretlAICC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return (-2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff) + ((2 * model->ncoeff * (model->ncoeff + 1)) / qreal(model->nobs - model->ncoeff - 1));
}

qreal ViArmaNoiseDetector::gretlBIC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + model->ncoeff * log(model->nobs);
}

qreal ViArmaNoiseDetector::gretlHQC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff * log(log(model->nobs));
}

bool ViArmaNoiseDetector::leastSquareFit(const qreal *input, const int &degree, qreal *coefficients)
{
	static int i;

	qreal **matrix = new qreal*[degree];
	for(i = 0; i < degree; ++i)
	{
		matrix[i] = new qreal[degree];
	}

	bool result = leastSquareFit(input, degree, coefficients, matrix);

	for(i = 0; i < degree; ++i)
	{
		delete [] matrix[i];
	}
	delete [] matrix;

	return result;
}

bool ViArmaNoiseDetector::leastSquareFit(const qreal *input, const int &degree, qreal *coefficients, qreal **matrix)
{
	static int i, j, k, hj, hi, end;

	for(i = 0; i < degree; ++i)
	{
		coefficients[i] = 0.0;
		for(j = 0; j < degree; ++j)
		{
			matrix[i][j] = 0.0;
		}
	}

	end = mWindowSize - 1;
	for(i = degree - 1; i < end; ++i)
	{
		hi = i + 1;
		for(j = 0; j < degree; ++j)
		{
			hj = i - j;
			coefficients[j] += (input[hi] * input[hj]);
			for(k = j; k < degree; ++k)
			{
				matrix[j][k] += (input[hj] * input[i - k]);
			}
		}
	}

	for(i = 0; i < degree; ++i)
	{
		coefficients[i] /= (mWindowSize - degree);
		for(j = i; j < degree; ++j)
		{
			matrix[i][j] /= (mWindowSize - degree);
			matrix[j][i] = matrix[i][j];
		}
	}

	return solveEquations(matrix, coefficients, degree);
}

bool ViArmaNoiseDetector::solveEquations(double **matrix, double *coefficients, const int &degree)
{
	static int i, j, k, maxi, end;
	static qreal vswap, max, h, pivot, q;
	static qreal *mswap, *hvec;

	end = degree - 1;
	for(i = 0; i < end; ++i)
	{
		max = fabs(matrix[i][i]);
		maxi = i;
		for(j = i + 1; j < degree; ++j)
		{
			if((h = fabs(matrix[j][i])) > max)
			{
				max = h;
				maxi = j;
			}
		}
		if(maxi != i)
		{
			mswap = matrix[i];
			matrix[i] = matrix[maxi];
			matrix[maxi] = mswap;
			vswap = coefficients[i];
			coefficients[i] = coefficients[maxi];
			coefficients[maxi] = vswap;
		}

		hvec = matrix[i];
		pivot = hvec[i];
		if(fabs(pivot) == 0.0) return false;

		for(j = i + 1; j < degree; ++j)
		{
			q = - matrix[j][i] / pivot;
			matrix[j][i] = 0.0;
			for(k = i + 1; k < degree; ++k)
			{
				matrix[j][k] += q * hvec[k];
			}
			coefficients[j] += (q * coefficients[i]);
		}
	}

	coefficients[end] /= matrix[end][end];
	for(i = degree - 2; i >= 0; --i)
	{
		hvec = matrix[i];
		for(j = end; j > i; --j)
		{
			coefficients[i] -= (hvec[j] * coefficients[j]);
		}
		coefficients[i] /= hvec[i];
	}
	return true;
}

qreal ViArmaNoiseDetector::mean(const qreal *samples) const
{
	static qreal m;
	static int i;

	m = 0;
	for(i = 0; i < mWindowSize; ++i) m += samples[i];

	return m / mWindowSize;
}

qreal ViArmaNoiseDetector::mean(const QQueue<qreal> &samples) const
{
	static qreal m;
	static int i;

	m = 0;
	for(i = 0; i < mWindowSize; ++i) m += samples[i];

	return m / mWindowSize;
}

qreal ViArmaNoiseDetector::variance(const qreal *samples, const qreal &mean) const
{
	static qreal var;
	static int i;

	var = 0;
	for(i = 0; i < mWindowSize; ++i) var += qPow((samples[i] - mean), 2);

	return var / mWindowSize;
}

qreal ViArmaNoiseDetector::variance(const QQueue<qreal> &samples, const qreal &mean) const
{
	static qreal var;
	static int i;

	var = 0;
	for(i = 0; i < mWindowSize; ++i) var += qPow((samples[i] - mean), 2);

	return var / mWindowSize;
}

qreal ViArmaNoiseDetector::autocorrelation(const int &lag, const qreal *samples, const qreal &mean, const qreal &variance) const
{
	static qreal autocv;
	static int i, end;

	autocv = 0;
	end = mWindowSize - lag;
	for(i = 0; i < end; ++i) autocv += (samples[i] - mean) * (samples[i+lag] - mean);
	autocv /= (mWindowSize - lag);

	return autocv / variance; // Autocorrelation is autocovariance divided by variance
}

void ViArmaNoiseDetector::partialAutocorrelation(qreal *partials, const qreal *samples, const qreal &mean, const qreal &variance) const
{
	static qreal sum1, sum2;
	static int i, j, end;

	for(i = 0; i <= mWindowSize; ++i)
	{
		for(j = 0; j <= mWindowSize; ++j)
		{
			mPartialMatrix[i][j] = 0;
		}
	}

	mPartialMatrix[1][1] = autocorrelation(1, samples, mean, variance);
	for (i = 2; i <= mWindowSize; ++i)
	{
		sum1 = 0;
		sum2 = 0;
		end = i - 1;

		for (j = 1; j <= end; ++j)
		{
			sum1 += mPartialMatrix[end][j] * autocorrelation(i - j, samples, mean, variance);
			sum2 += mPartialMatrix[end][j] * autocorrelation(j, samples, mean, variance);
		}

		mPartialMatrix[i][i] = (autocorrelation(i, samples, mean, variance) - sum1) / (1 - sum2);

		for (j = 1; j <= end; j++)
		{
			mPartialMatrix[i][j] = mPartialMatrix[end][j] - mPartialMatrix[i][i] * mPartialMatrix[end][i - j];
		}
	}

	for (i = 0; i < mWindowSize; ++i)
	{
		partials[i] = mPartialMatrix[i][i];
	}
}

ViArmaNoiseDetector* ViArmaNoiseDetector::clone()
{
	return new ViArmaNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViArmaNoiseDetector* create()
{
	return new ViArmaNoiseDetector();
}

#ifdef __cplusplus
}
#endif
