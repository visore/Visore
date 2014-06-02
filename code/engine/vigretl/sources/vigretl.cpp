#include <vigretl.h>
#include <vilogger.h>
#include <QFile>
#include <QtMath>
#include <QVector>

#define LN2PI1					2.837877066409345 // log(2*pi) + 1

#define DEFAULT_ESTIMATION		CML
#define DEFAULT_CRITERIA		None

#define DEFAULT_AR_DEGREE		5
#define DEFAULT_I_DEGREE		0
#define DEFAULT_MA_DEGREE		0
#define DEFAULT_GARCH_DEGREE	1
#define DEFAULT_ARCH_DEGREE		1

#define MAXIMUM_ITERATIONS		100
#define MAXIMUM_TEST_AR_DEGREE	20
#define MAXIMUM_TEST_MA_DEGREE	0

bool ViGretl::mInitialized = false;

ViGretl::ViGretl(const Type &type)
{
	initialize();

	mGretlParameters = NULL;
	mGretlEstimation = OPT_NONE;

	mType = type;
	if(mType == MA || mType == AR || mType == I || mType == ARMA || mType == ARIMA)
	{
		// ARIMA (AR Order, Integrated Order, MA Order, Seperator, Index of dependent variable)
		mGretlParameters = gretl_list_new(5);
		mGretlParameters[1] = 0;
		mGretlParameters[2] = 0;
		mGretlParameters[3] = 0;
		mGretlParameters[4] = LISTSEP;
		mGretlParameters[5] = 1;

		estimatePointer = &ViGretl::estimateArima;
		fixedModelPointer = &ViGretl::fixedArimaModel;
	}
	else if(mType == ARCH || mType == GARCH)
	{
		// GARCH (GARCH Order, ARCH Order, Seperator, Index of dependent variable)
		mGretlParameters = gretl_list_new(4);
		mGretlParameters[1] = 0;
		mGretlParameters[2] = 0;
		mGretlParameters[3] = LISTSEP;
		mGretlParameters[4] = 1;

		if(mType == ARCH) estimatePointer = &ViGretl::estimateArch;
		else if(mType == GARCH) estimatePointer = &ViGretl::estimateGarch;
		fixedModelPointer = &ViGretl::fixedGarchModel;
	}

	mMaDegree = 0;
	mIDegree = 0;
	mArDegree = 0;
	mArchDegree = 0;
	mGarchDegree = 0;
	setDegree(AR, DEFAULT_AR_DEGREE);
	setDegree(I, DEFAULT_I_DEGREE);
	setDegree(MA, DEFAULT_MA_DEGREE);
	setDegree(ARCH, DEFAULT_ARCH_DEGREE);
	setDegree(GARCH, DEFAULT_GARCH_DEGREE);

	setEstimation(DEFAULT_ESTIMATION);
	setCriteria(DEFAULT_CRITERIA);
}

ViGretl::ViGretl(const ViGretl &other)
{
	initialize();

	mGretlParameters = gretl_list_copy(other.mGretlParameters);
	mGretlEstimation = OPT_NONE;
	mType = other.mType;
	if(mType == MA || mType == AR || mType == I || mType == ARMA || mType == ARIMA)
	{
		estimatePointer = &ViGretl::estimateArima;
		fixedModelPointer = &ViGretl::fixedArimaModel;
	}
	else if(mType == ARCH || mType == GARCH)
	{
		estimatePointer = &ViGretl::estimateGarch;
		fixedModelPointer = &ViGretl::fixedGarchModel;
	}

	mMaDegree = 0;
	mIDegree = 0;
	mArDegree = 0;
	mArchDegree = 0;
	mGarchDegree = 0;
	setDegree(AR, other.mArDegree);
	setDegree(I, other.mIDegree);
	setDegree(MA, other.mMaDegree);
	setDegree(ARCH, other.mArchDegree);
	setDegree(GARCH, other.mGarchDegree);

	setEstimation(other.mEstimation);
	setCriteria(other.mCriteria);
}

ViGretl::~ViGretl()
{
	if(mGretlParameters != NULL)
	{
		free(mGretlParameters);
		mGretlParameters = NULL;
	}
}

bool ViGretl::initialize(const bool &enableOutput)
{
	if(mInitialized) return false;
	mInitialized = true;

	libgretl_init();
	if(!enableOutput) freopen("/dev/null", "w", stderr); // Stop forced gretl error output

	libset_set_int(BFGS_MAXITER, MAXIMUM_ITERATIONS); // ML
	libset_set_int(BHHH_MAXITER, MAXIMUM_ITERATIONS); // CML

	return mInitialized;
}

bool ViGretl::uninitialize()
{
	if(!mInitialized) return false;
	mInitialized = false;
	libgretl_cleanup();
	return mInitialized;
}

void ViGretl::setEstimation(const Estimation &estimation)
{
	mEstimation = estimation;
	if(mEstimation == ExactMaximumLikelihoodLimited) mGretlEstimation = OPT_L;
	else if(mEstimation == ConditionalMaximumLikelihood) mGretlEstimation = OPT_C;
	else if(mEstimation == X12Arma)
	{

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

void ViGretl::setCriteria(const Criteria &criteria)
{
	mCriteria = criteria;
	if(mType == MA || mType == AR || mType == I || mType == ARMA || mType == ARIMA)
	{
		if(mCriteria == None) modelPointer = &ViGretl::fixedArimaModel;
		else if(mCriteria == AutoCorrelation) modelPointer = &ViGretl::autocorrelationModel;
		else if(mCriteria) modelPointer = &ViGretl::bestModel;
	}
	else if(mType == ARCH || mType == GARCH)
	{
		if(mCriteria == None) modelPointer = &ViGretl::fixedGarchModel;
		else
		{
			LOG("Not implemented for GARCH yet"); exit(-1);
		}
	}

	if(mCriteria == MSE) orderPointer = &ViGretl::calculateMSE;
	else if(mCriteria == R2) orderPointer = &ViGretl::calculateR2;
	else if(mCriteria == AR2) orderPointer = &ViGretl::calculateAR2;
	else if(mCriteria == AIC) orderPointer = &ViGretl::calculateAIC;
	else if(mCriteria == AICC) orderPointer = &ViGretl::calculateAICC;
	else if(mCriteria == BIC) orderPointer = &ViGretl::calculateBIC;
	else if(mCriteria == HQC) orderPointer = &ViGretl::calculateHQC;
}

bool ViGretl::setDegree(const Type &type, const int &degree)
{
	if(mType == AR && type != AR) return false;
	else if(mType == I && type != I) return false;
	else if(mType == MA && type != MA) return false;
	else if(mType == ARMA && !(type == AR || type == MA)) return false;
	else if(mType == ARIMA && !(type == AR || type == I || type == MA)) return false;
	else if(mType == ARCH && type != ARCH) return false;
	else if(mType == GARCH && !(type == ARCH || type == GARCH)) return false;

	if(type == AR)
	{
		mArDegree = degree;
		mGretlParameters[1] = mArDegree;
	}
	else if(type == I)
	{
		mIDegree = degree;
		mGretlParameters[2] = mIDegree;
	}
	else if(type == MA)
	{
		mMaDegree = degree;
		mGretlParameters[3] = mMaDegree;
	}
	else if(type == ARCH)
	{
		mArchDegree = degree;
		mGretlParameters[2] = mArchDegree;
	}
	else if(type == GARCH)
	{
		mGarchDegree = degree;
		mGretlParameters[1] = mGarchDegree;
	}
	return true;
}

int ViGretl::degree(const Type &type)
{
	if(type == AR) return mArDegree;
	else if(type == I) return mIDegree;
	else if(type == MA) return mMaDegree;
	else if(type == ARCH) return mArchDegree;
	else if(type == GARCH) return mGarchDegree;
}

void ViGretl::adjustWindowSize(int &windowSize)
{
	if(mGretlEstimation == OPT_X && windowSize > 600)
	{
		STATICLOG("X12Arima can have a maximum window size of 600. Resetting it 600.", QtCriticalMsg, "ViGretl");
		windowSize = 600;
	}
}

int* ViGretl::adjustParameters(int *parameters)
{
	//Some parameters cause gretl to segfault

	if(mType == ARMA && parameters[1] == 19 && parameters[3] == 2) parameters[1];

	return parameters;
}

MODEL* ViGretl::estimate(DATASET *data)
{
	 MODEL *model = (this->*modelPointer)(data);
	 if(model != NULL && model->errcode)
	 {
		 gretl_model_free(model);
		 model = NULL;
	 }
	 return model;
}

FITRESID* ViGretl::forecast(DATASET *data, MODEL *model)
{
	return get_forecast(model, data->t2 + 1, data->n - 1, 0, data, OPT_NONE, NULL);
}

bool ViGretl::forecast(DATASET *data, MODEL *model, qreal *output, const int &size)
{
	static int i, start, error;
	if(model == NULL)
	{
		for(i = 0; i < size; ++i) output[i] = 0;
		return false;
	}
	error = 0; // Gretl doesn't do this for us
	FITRESID *cast = get_forecast(model, data->t2 + 1, data->n - 1, 0, data, OPT_NONE, &error);
	if(error)
	{
		for(i = 0; i < size; ++i) output[i] = 0;
		return false;
	}
	start = data->t2 + 1;
	for(i = 0; i < size; ++i) output[i] = cast->fitted[start + i];
	free_fit_resid(cast);
	return true;
}

bool ViGretl::backcast(DATASET *data, MODEL *model, qreal *output, const int &size)
{
	static int i, start, end, error;
	if(model == NULL)
	{
		for(i = 0; i < size; ++i) output[i] = 0;
		return false;
	}
	error = 0; // Gretl doesn't do this for us
	FITRESID *cast = get_forecast(model, data->t2 + 1, data->n - 1, 0, data, OPT_NONE, &error);
	if(error)
	{
		for(i = 0; i < size; ++i) output[i] = 0;
		return false;
	}
	start = data->t2 + 1;
	end = size - 1;
	for(i = 0; i < size; ++i) output[end - i] = cast->fitted[start + i];
	free_fit_resid(cast);
	return true;
}

bool ViGretl::forecast(const qreal *input, const int &inputSize, qreal *output, const int &outputSize)
{
	static bool result;
	DATASET *data = create(input, inputSize, outputSize);
	MODEL *model = estimate(data);
	result = forecast(data, model, output, outputSize);
	ViGretl::clear(model);
	ViGretl::clear(data);
	return result;
}

bool ViGretl::backcast(const qreal *input, const int &inputSize, qreal *output, const int &outputSize)
{
	static bool result;
	DATASET *data = createReversed(input, inputSize, outputSize);
	MODEL *model = estimate(data);
	result = backcast(data, model, output, outputSize);
	ViGretl::clear(model);
	ViGretl::clear(data);
	return result;
}

bool ViGretl::interpolate(const qreal *left, const int &leftSize, const qreal *right, const int &rightSize, qreal *output, const int &outputSize)
{
	DATASET *data = create_new_dataset(2, leftSize + outputSize + rightSize, 0);

	static int i;
	for(i = 0; i < leftSize; ++i) data->Z[1][i] = left[i];
	for(i = 0; i < outputSize; ++i) data->Z[1][i + leftSize] = -999;
	for(i = 0; i < rightSize; ++i) data->Z[1][i+ leftSize+outputSize] = right[i];

	int miss = set_miss (NULL, "-999",data,NULL);

MODEL *model;
*model = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
if(model->errcode) cout<<"could not estimate: "<<model->errcode<<"\t"<<errmsg_get_with_default(model->errcode)<<endl;

	static int start, error;
	error = 0; // Gretl doesn't do this for us
	FITRESID *cast = get_forecast(model, data->t1, data->t2, 0, data, OPT_NONE, &error);
	if(error)
	{
		cout<<"could not forecast: "<<error<<endl;
		for(i = 0; i < outputSize; ++i) output[i] = 0;
		return false;
	}
	for(i = 0; i < outputSize; ++i) output[i] = cast->fitted[leftSize + i];
	free_fit_resid(cast);
	return true;

}

DATASET* ViGretl::create(const qreal *input, const int &inputSize, const int &outputSize)
{
	DATASET *data = create_new_dataset(2, inputSize + outputSize, 0);
	strcpy(data->varname[1], "visoregretldata"); // For X12 we need the series to have a name
	data->t2 = data->n - 1 - outputSize;

	static int i;
	for(i = 0; i < inputSize; ++i) data->Z[1][i] = input[i];

	return data;
}

DATASET* ViGretl::createReversed(const qreal *input, const int &inputSize, const int &outputSize)
{
	DATASET *data = create_new_dataset(2, inputSize + outputSize, 0);
	strcpy(data->varname[1], "visoregretldata"); // For X12 we need the series to have a name
	data->t2 = data->n - 1 - outputSize;

	static int i, end;
	end = inputSize - 1;
	for(i = 0; i < inputSize; ++i) data->Z[1][i] = input[end - i];

	return data;
}

DATASET* ViGretl::create(const int &inputSize, const int &outputSize)
{
	DATASET *data = create_new_dataset(2, inputSize + outputSize, 0);
	strcpy(data->varname[1], "visoregretldata"); // For X12 we need the series to have a name
	data->t2 = data->n - 1 - outputSize;
	return data;
}

void ViGretl::clear(DATASET *data)
{
	destroy_dataset(data);
}

void ViGretl::clear(MODEL *model)
{
	gretl_model_free(model);
}

void ViGretl::clear(FITRESID *cast)
{
	free_fit_resid(cast);
}

MODEL* ViGretl::fixedArimaModel(DATASET *data)
{
	MODEL *model;
	int *parameters = gretl_list_copy(mGretlParameters);
	while(true)
	{
		model = gretl_model_new();
		(this->*estimatePointer)(model, data, parameters);
		if(model->errcode)
		{
			gretl_model_free(model);
			if(parameters[3] > 0) --parameters[3];
			else if(parameters[2] > 0)
			{
				parameters[3] = mMaDegree;
				--parameters[2];
			}
			else if(parameters[1] > 0)
			{
				parameters[2] = mIDegree;
				parameters[3] = mMaDegree;
				--parameters[1];
			}
			else
			{
				model = NULL;
				break;
			}
		}
		else break;
	}
	free(parameters);
	return model;
}

MODEL* ViGretl::fixedGarchModel(DATASET *data)
{
	MODEL *model;
	int *parameters = gretl_list_copy(mGretlParameters);
	while(true)
	{
		model = gretl_model_new();
		(this->*estimatePointer)(model, data, parameters);
		if(model->errcode)
		{
			gretl_model_free(model);
			if(parameters[1] > 0) --parameters[1];
			else if(parameters[2] > 0)
			{
				parameters[1] = mGarchDegree;
				--parameters[2];
			}
			else
			{
				model = NULL;
				break;
			}
		}
		else break;
	}
	free(parameters);
	return model;
}

MODEL* ViGretl::autocorrelationModel(DATASET *data)
{
	static int error, i, arDegree, maDegree, size;

	size = data->t2 + 1;

	error = 0; // We have to set this manually, because gretl doesn't
	gretl_matrix *matrix = acf_matrix(data->Z[1], qMax(MAXIMUM_TEST_AR_DEGREE, MAXIMUM_TEST_MA_DEGREE), NULL, size, &error);

	// ************************************* NEED to adapt this for ARIMA and GARCH
	if(error) return (this->*fixedModelPointer)(data);

	maDegree = 0;
	qreal acf = 0, acfConfidenceLevel;
	for(i = 0; i < matrix->rows; ++i)
	{
		acfConfidenceLevel = 1.96 * qSqrt((1 + (2 * acf)) / size);
		if(qAbs(gretl_matrix_get(matrix, i, 0)) < acfConfidenceLevel)
		{
			maDegree = i;
			break;
		}
		acf += qPow(gretl_matrix_get(matrix, i, 0), 2);
	}

	arDegree = 0;
	qreal pacfConfidenceLevel = 1.96 / qSqrt(size - 1);
	for(i = 0; i < matrix->rows; ++i)
	{
		if(qAbs(gretl_matrix_get(matrix, i, 1)) < pacfConfidenceLevel)
		{
			arDegree = i;
			break;
		}
	}

	gretl_matrix_free(matrix);

	mGretlParameters[1] = arDegree;
	mGretlParameters[3] = maDegree;
	return (this->*fixedModelPointer)(data);
}

MODEL* ViGretl::bestModel(DATASET *data)
{
	qreal score, minScore = DBL_MAX;
	MODEL *bestModel = NULL;
	int *parameters = gretl_list_copy(mGretlParameters);

	for(int i = 0; i <= MAXIMUM_TEST_AR_DEGREE; ++i)
	{
		parameters[1] = i;
		for(int j = 0; j <= MAXIMUM_TEST_MA_DEGREE; ++j)
		{
			parameters[3] = j;
			MODEL *tempModel = gretl_model_new();
			(this->*estimatePointer)(tempModel, data, parameters);
			score = (this->*orderPointer)(tempModel);
			if(score < minScore)
			{
				minScore = score;
				gretl_model_free(bestModel);
				bestModel = tempModel;
			}
			else gretl_model_free(tempModel);
		}
	}

	free(parameters);

	//cout<<"ARMA("<<bestAr<<", "<<bestMa<<")"<<endl;
	return bestModel;
}

void ViGretl::estimateArima(MODEL *model, DATASET *data, int *parameters)
{
	*model = arma_model(adjustParameters(parameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
}

void ViGretl::estimateGarch(MODEL *model, DATASET *data, int *parameters)
{
	*model = garch_model(adjustParameters(parameters), data, NULL, (gretlopt) mGretlEstimation);
}

void ViGretl::estimateArch(MODEL *model, DATASET *data, int *parameters)
{
	*model = garch_model(adjustParameters(parameters), data, NULL, (gretlopt) mGretlEstimation);
	//*model = arch_model(parameters, parameters[2], data, (gretlopt) mGretlEstimation);
}

qreal ViGretl::calculateMSE(MODEL *model)
{
	return model->ess;
}

qreal ViGretl::calculateR2(MODEL *model)
{
	return model->rsq;
}

qreal ViGretl::calculateAR2(MODEL *model)
{
	return model->adjrsq;
}

qreal ViGretl::calculateAIC(MODEL *model)
{
	return model->criterion[C_AIC];
}

qreal ViGretl::calculateAICC(MODEL *model)
{
	if(model->criterion[C_AIC] == DBL_MAX) return DBL_MAX;
	return model->criterion[C_AIC] + ((2 * model->ncoeff * (model->ncoeff + 1)) / qreal(model->nobs - model->ncoeff - 1));
}

qreal ViGretl::calculateBIC(MODEL *model)
{
	return model->criterion[C_BIC];
}

qreal ViGretl::calculateHQC(MODEL *model)
{
	return model->criterion[C_HQC];
}
