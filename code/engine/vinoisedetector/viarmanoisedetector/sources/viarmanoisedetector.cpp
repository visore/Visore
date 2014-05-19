#include <viarmanoisedetector.h>

#define TWO_PI 6.2831853071795864769252866
#define LN2PI1 2.837877066409345 // log(2*pi) + 1

#define DEFAULT_AR_DEGREE 5
#define DEFAULT_MA_DEGREE 0

#define WINDOW_SIZE 2048

#define DEFAULT_GRETL_CRITERIA None

#define MAXIMUM_ITERATIONS 1
#define MAXIMUM_TEST_DEGREE 5

#define NOISE_CUTOFF 0.2

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
	mHasNoise = false;

	addParameterName("Window Size");
	addParameterName("AR Order");
	addParameterName("MA Order");
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
	mHasNoise = false;
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

void ViArmaNoiseDetector::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViArmaNoiseDetector::setParameters(const qreal &param1, const qreal &param2)
{
	setWindowSize(param1);
	setDegree(mType, param2);
}

void ViArmaNoiseDetector::setParameters(const qreal &param1, const qreal &param2, const qreal &param3)
{
	setWindowSize(param1);
	setDegree(param2, param3);
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
		int forecast = 50;
		if(mGretlData != NULL) destroy_dataset(mGretlData);
		mGretlData = create_new_dataset(2, mWindowSize + forecast, 0); // + 1 for out-of-sample prediction
		strcpy(mGretlData->varname[1], "visoredata"); // For X12 we need the series to have a name
		mGretlData->t2 = mGretlData->n - 1 - forecast; // reserve 1 observations for out-of-sample forecasting
	}

	mPacfConfidenceLevel = 1.96 / qSqrt(mWindowSize - 1);
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

void ViArmaNoiseDetector::setDegree(const int &arDegree, const int &maDegree)
{
	setDegree(AR, arDegree);
	setDegree(MA, maDegree);
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

	mHasNoise = false;
}

void ViArmaNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	(this->*calculateNoisePointer)(samples);
}

void ViArmaNoiseDetector::calculateNoiseNative(QQueue<qreal> &samples)
{
	static bool failedAR, failedMA;
	static int i;
	static qreal theMean, theVariance, difference;

	while(samples.size() > mWindowSize)
	{
		failedAR = failedMA = false;

		if(mHasNoise)
		{
			samples[mWindowSize - 1] = mPrediction;
			mHasNoise = false;
		}
		mPrediction = 0;

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
				//mPrediction += generateNoise(variance); // This extra error term slightly reduces the accuracy
				for(i = 0; i < mMaDegree; ++i)
				{
					mPrediction += mMaCoefficients[i] * mWindowData[mWindowSize - 1 - i];
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
					mPrediction += (mArCoefficients[i] * mWindowData[mWindowSize - 1 - i]);
				}
			}
			else failedAR = true;

		}
		if(failedAR && failedMA) setNoise(0);
		else
		{
			difference = qAbs(samples[mWindowSize] - mPrediction);
			if(difference > NOISE_CUTOFF) mHasNoise = true;
			setNoise(difference);
		}
		samples.removeFirst();
	}
}

void ViArmaNoiseDetector::calculateNoiseGretl(QQueue<qreal> &samples)
{
	static bool failed;
		static int i, error;
		static qreal difference;

		while(samples.size() > mWindowSize+49)
		{
			failed = false;

			mPrediction = 0;

			for(i = 0; i < mWindowSize; ++i)
			{
				mGretlData->Z[1][i] = samples[i];
			}

			MODEL *model = gretl_model_new();
			(this->*gretlModelPointer)(model);

			if(model->errcode) failed = true;
			else
			{
				/*error = 0; // gretl doesn't do this for us. We manually have to start with "no" error.
				FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
				if(error) failed = true;
				else
				{
					if(forecast->nobs > mWindowSize) mPrediction = forecast->fitted[mWindowSize];
					free_fit_resid(forecast);
				}*/

				error = 0; // gretl doesn't do this for us. We manually have to start with "no" error.
				FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
				if(error) failed = true;
				else
				{
					int counter = 0;
					qreal diff = 0.5l;

					do
					{
						mPrediction = forecast->fitted[mWindowSize+counter];
						difference = qAbs(samples[mWindowSize+counter] - mPrediction);
//cout<<mPrediction<<"\t"<<samples[mWindowSize+counter]<<"\t"<<difference<<endl;
						setNoise(difference);

if(difference>diff)samples[mWindowSize+counter]=samples[mWindowSize+counter-1];
						++counter;


					}
					while(difference > diff && counter < 50);
					for(i=0;i<counter;++i)samples.removeFirst();
					//for(i=samples.size()-1;i>=0;--i)samples[i]

					free_fit_resid(forecast);
				}
			}
			gretl_model_free(model);

			if(failed)
			{
				setNoise(0);
				samples.removeFirst();
			}
			/*else
			{
				difference = qAbs(samples[mWindowSize] - mPrediction);
				if(difference > NOISE_CUTOFF) mHasNoise = true;
				setNoise(difference);
				//setNoise(mPrediction);
			}
			samples.removeFirst();*/

		}














	/*static bool failed;
	static int i, error;
	static qreal difference;

	while(samples.size() > mWindowSize)
	{
		failed = false;

		if(mHasNoise)
		{
			//if(mPrediction > 1) samples[mWindowSize - 1] = 1;
			//else samples[mWindowSize - 1] = mPrediction;

			if(qAbs(samples[mWindowSize - 2] - mPrediction) < 0.1)
			{
				samples[mWindowSize - 1] = mPrediction;
			}
			else
			{
				qreal m = 0;
				for(i = 0; i < mWindowSize-1; ++i)
				{
					m+=samples[i];
				}
				m /= (mWindowSize-1);
				samples[mWindowSize - 1] = m;
			}

			mHasNoise = false;
		}
		mPrediction = 0;

		for(i = 0; i < mWindowSize; ++i)
		{
			mGretlData->Z[1][i] = samples[i];
		}

		MODEL *model = gretl_model_new();
		(this->*gretlModelPointer)(model);

		if(model->errcode) failed = true;
		else
		{
			error = 0; // gretl doesn't do this for us. We manually have to start with "no" error.
			FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
			if(error) failed = true;
			else
			{
				if(forecast->nobs > mWindowSize) mPrediction = forecast->fitted[mWindowSize];
				free_fit_resid(forecast);
			}
		}
		gretl_model_free(model);

		if(failed)
		{
			setNoise(0);
		}
		else
		{
			difference = qAbs(samples[mWindowSize] - mPrediction);
			if(difference > NOISE_CUTOFF) mHasNoise = true;
			setNoise(difference);
			//setNoise(mPrediction);
		}
		samples.removeFirst();
	}*/
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
			else break;
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
	qreal acf = 0, acfConfidenceLevel;
	for(i = 0; i < matrix->rows; ++i)
	{
		acfConfidenceLevel = 1.96 * qSqrt((1 + (2 * acf)) / mWindowSize);
		if(abs(gretl_matrix_get(matrix, i, 0)) < acfConfidenceLevel)
		{
			maDegree = i;
			break;
		}
		acf += qPow(gretl_matrix_get(matrix, i, 0), 2);
	}

	arDegree = 0;
	for(i = 0; i < matrix->rows; ++i)
	{
		if(abs(gretl_matrix_get(matrix, i, 1)) < mPacfConfidenceLevel)
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
			else break;
		}
		else break;
	}
//	cout<<"ARMA("<<mGretlParameters[1]<<", "<<mGretlParameters[3]<<")"<<endl;
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

	/*min = DBL_MAX;
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
	}*/

	if(arOrderBest < 0) arOrderBest = 0;
	if(maOrderBest < 0) maOrderBest = 0;
	mGretlParameters[1] = arOrderBest;
	mGretlParameters[3] = maOrderBest;

	//cout<<"ARMA("<<mGretlParameters[1]<<", "<<mGretlParameters[3]<<")"<<endl;

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
