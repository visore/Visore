/*#include <viarmainterpolator.h>
#include <vilogger.h>

#define LN2PI1 2.837877066409345 // log(2*pi) + 1

#define DEFAULT_WINDOW_SIZE 1024
#define DEFAULT_TYPE ARMA
#define DEFAULT_ESTIMATION CML
#define DEFAULT_CRITERIA None

#define DEFAULT_AR_DEGREE 5
#define DEFAULT_MA_DEGREE 0

#define MAXIMUM_ITERATIONS 10

#define MAXIMUM_TEST_AR_DEGREE 18 // 19 and above gives gretl seg faults
#define MAXIMUM_TEST_MA_DEGREE 5

ViArmaInterpolator::ViArmaInterpolator()
	: ViInterpolator()
{
	setType(DEFAULT_TYPE);
	setEstimation(DEFAULT_ESTIMATION);
	setCriteria(DEFAULT_CRITERIA);
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setDegree(DEFAULT_AR_DEGREE, DEFAULT_MA_DEGREE);

	mGretlParameters = NULL;
	initialize();
}

ViArmaInterpolator::~ViArmaInterpolator()
{
	clear();
}

void ViArmaInterpolator::setType(const Type &type)
{
	mType = type;
	if(mType == MA) mGretlParameters[1] = 0; // Set AR order to 0
	else if(mType == AR) mGretlParameters[3] = 0; // Set MA order to 0
}

void ViArmaInterpolator::setEstimation(const Estimation &estimation)
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


void ViArmaInterpolator::setCriteria(const Criteria &criteria)
{
	mCriteria = criteria;
	if(mCriteria == None)
	{
		modelPointer = &ViArmaInterpolator::fixedModel;
	}
	else if(mCriteria == AutoCorrelation)
	{
		modelPointer = &ViArmaInterpolator::autocorrelationModel;
	}
	else
	{
		modelPointer = &ViArmaInterpolator::bestModel;
		if(mCriteria == R2)
		{
			orderPointer = &ViArmaInterpolator::calculateR2;
		}
		else if(mCriteria == AR2)
		{
			orderPointer = &ViArmaInterpolator::calculateAR2;
		}
		else if(mCriteria == AIC)
		{
			orderPointer = &ViArmaInterpolator::calculateAIC;
		}
		else if(mCriteria == AICC)
		{
			orderPointer = &ViArmaInterpolator::calculateAICC;
		}
		else if(mCriteria == BIC)
		{
			orderPointer = &ViArmaInterpolator::calculateBIC;
		}
		else if(mCriteria == HQC)
		{
			orderPointer = &ViArmaInterpolator::calculateHQC;
		}
	}
}

void ViArmaInterpolator::setWindowSize(const int &size)
{
	if(mGretlEstimation == OPT_X && size > 600)
	{
		LOG("X12Arima can have a maximum window size of 600. Resetting it 600.", QtCriticalMsg);
		ViInterpolator::setWindowSize(600);
	}
	else
	{
		ViInterpolator::setWindowSize(size);
	}
}

void ViArmaInterpolator::setDegree(const Type &type, const int &degree)
{
	if(type == MA || type == ARMA) mMaDegree = degree;
	if(type == AR || type == ARMA) mArDegree = degree;
}

void ViArmaInterpolator::setDegree(const int &arDegree, const int &maDegree)
{
	setDegree(AR, arDegree);
	setDegree(MA, maDegree);
}

void ViArmaInterpolator::setParameters(const qreal &param1, const qreal &param2, const qreal &param3)
{
	setWindowSize(param1);
	setDegree(param2, param3);
}

void ViArmaInterpolator::initialize()
{
	ViGretl::initialize();

	libset_set_int(BFGS_MAXITER, MAXIMUM_ITERATIONS); // ML
	libset_set_int(BHHH_MAXITER, MAXIMUM_ITERATIONS); // CML

	mGretlParameters = gretl_list_new(5);
	mGretlParameters[1] = 0;        // AR order
	mGretlParameters[2] = 0;        // order of integration
	mGretlParameters[3] = 0;        // MA order
	mGretlParameters[4] = LISTSEP;  // separator
	mGretlParameters[5] = 1;
}

void ViArmaInterpolator::clear()
{
	if(mGretlParameters != NULL)
	{
		free(mGretlParameters);
		mGretlParameters = NULL;
	}
}

int* ViArmaInterpolator::adjustParameters(int *parameters)
{
	//Some parameters cause gretl to segfault

	if(parameters[1] == 19 && parameters[3] == 2) parameters[1];

	return parameters;
}

bool ViArmaInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, error, realSize;

	DATASET *gretlData;
	MODEL *model;

	qreal output[outputSize];
	for(i = 0; i < outputSize; ++i) output[i] = 0;
	int count[outputSize];

	// Forecast

	realSize = qMin(leftSize, mWindowSize);
	gretlData = create_new_dataset(2, realSize + outputSize, 0);
	//strcpy(gretlData2->varname[1], "visoredata"); // For X12 we need the series to have a name
	gretlData->t2 = gretlData->n - 1 - outputSize;

	for(i = realSize - 1; i >= 0; --i) gretlData->Z[1][i] = leftSamples[i];

	model = (this->*modelPointer)(gretlData);
	if(!model->errcode)
	{
		error = 0; // gretl doesn't do this for us. We manually have to start with "no" error.
		FITRESID *forecast = get_forecast(model, gretlData->t2 + 1, gretlData->n - 1, 0, gretlData, OPT_NONE, &error);
		if(!error)
		{
			for(i = 0; i < outputSize; ++i)
			{
				output[i] += forecast->fitted[realSize + i];
				count[i] += 1;
			}
			free_fit_resid(forecast);
		}
	}

	gretl_model_free(model);
	destroy_dataset(gretlData);

	// Backcast

	realSize = qMin(rightSize, mWindowSize);
	gretlData = create_new_dataset(2, realSize + outputSize, 0);
	//strcpy(gretlData2->varname[1], "visoredata"); // For X12 we need the series to have a name
	gretlData->t2 = gretlData->n - 1 - outputSize;

	for(i = 0; i < realSize; ++i) gretlData->Z[1][i] = rightSamples[realSize - 1 - i];

	model = (this->*modelPointer)(gretlData);
	if(!model->errcode)
	{
		error = 0; // gretl doesn't do this for us. We manually have to start with "no" error.
		FITRESID *forecast = get_forecast(model, gretlData->t2 + 1, gretlData->n - 1, 0, gretlData, OPT_NONE, &error);
		if(!error)
		{
			for(i = 0; i < outputSize; ++i)
			{
				output[outputSize-i-1] += forecast->fitted[realSize + i];
				count[i] += 1;
			}
			free_fit_resid(forecast);
		}
	}

	gretl_model_free(model);
	destroy_dataset(gretlData);

	// Average forecast and backcast
	for(i = 0; i < outputSize; ++i)
	{
		if(count[i]) outputSamples[i] = output[i] / count[i];
	}

	return true;
}

MODEL* ViArmaInterpolator::fixedModel(DATASET *data)
{
	mGretlParameters[1] = mArDegree;;
	mGretlParameters[3] = mMaDegree;
	MODEL *model = gretl_model_new();

	while(true)
	{
		*model = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
		if(model->errcode)
		{
			if(mGretlParameters[3] > 0)
			{
				--mGretlParameters[3];
			}
			else if(mGretlParameters[1] > 0)
			{
				mGretlParameters[3] = 0;//mMaDegree;
				--mGretlParameters[1];
			}
			else break;
		}
		else break;
	}

	return model;
}

MODEL* ViArmaInterpolator::autocorrelationModel(DATASET *data)
{
	static int error, i, arDegree, maDegree, size;

	size = data->t2 + 1;

	error = 0; // We have to set this manually, because gretl doesn't
	gretl_matrix *matrix = acf_matrix(data->Z[1], qMax(MAXIMUM_TEST_AR_DEGREE, MAXIMUM_TEST_MA_DEGREE), NULL, size, &error);

	if(error) return fixedModel(data);

	maDegree = 0;
	qreal acf = 0, acfConfidenceLevel;
	for(i = 0; i < matrix->rows; ++i)
	{
		acfConfidenceLevel = 1.96 * qSqrt((1 + (2 * acf)) / size);
		if(abs(gretl_matrix_get(matrix, i, 0)) < acfConfidenceLevel)
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
		if(abs(gretl_matrix_get(matrix, i, 1)) < pacfConfidenceLevel)
		{
			arDegree = i;
			break;
		}
	}

	gretl_matrix_free(matrix);

	mGretlParameters[1] = arDegree;
	mGretlParameters[3] = maDegree;
	MODEL *model = gretl_model_new();

	while(true)
	{
		*model = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
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

	return model;
}

MODEL* ViArmaInterpolator::bestModel(DATASET *data)
{
	QVector<QVector<qreal>> scores(MAXIMUM_TEST_AR_DEGREE + 1);
	QVector<QVector<MODEL*>> models(MAXIMUM_TEST_AR_DEGREE + 1);

	for(int i = 0; i <= MAXIMUM_TEST_AR_DEGREE; ++i)
	{
		QVector<qreal> row1(MAXIMUM_TEST_MA_DEGREE + 1);
		row1.fill(DBL_MAX);
		scores[i] = row1;

		QVector<MODEL*> row2(MAXIMUM_TEST_MA_DEGREE + 1);
		row2.fill(NULL);
		models[i] = row2;
	}

	for(int i = 0; i <= MAXIMUM_TEST_AR_DEGREE; ++i)
	{
		mGretlParameters[1] = i;
		for(int j = 0; j <= MAXIMUM_TEST_MA_DEGREE; ++j)
		{
			mGretlParameters[3] = j;
			MODEL *tempModel = gretl_model_new();
			*tempModel = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
			scores[i][j] = (this->*orderPointer)(tempModel);
			models[i][j] = tempModel;
		}
	}

	qreal minScore = DBL_MAX;
	MODEL *bestModel;
	int bestAr = 0, bestMa = 0;
	for(int i = 0; i <= MAXIMUM_TEST_AR_DEGREE; ++i)
	{
		for(int j = 0; j <= MAXIMUM_TEST_MA_DEGREE; ++j)
		{
			if(!(i == 0 && j == 0) && scores[i][j] < minScore)
			{
				minScore = scores[i][j];
				bestModel = models[i][j];
				bestAr = i;
				bestMa = j;
			}
		}
	}

	for(int i = 0; i <= MAXIMUM_TEST_AR_DEGREE; ++i)
	{
		for(int j = 0; j <= MAXIMUM_TEST_MA_DEGREE; ++j)
		{
			if(i != bestAr && j != bestMa) gretl_model_free(models[i][j]);
		}
	}

	cout<<"ARMA("<<bestAr<<", "<<bestMa<<")"<<endl;
	return bestModel;
*/

	/*static qreal current, min;
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
		*tempModel = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
		current = (this->*orderPointer)(tempModel);
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
		*tempModel = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);
		current = (this->*orderPointer)(tempModel);
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

	cout<<"ARMA("<<mGretlParameters[1]<<", "<<mGretlParameters[3]<<")"<<endl;

	*model = arma_model(adjustParameters(mGretlParameters), NULL, data, (gretlopt) mGretlEstimation, NULL);*/
/*}

qreal ViArmaInterpolator::calculateR2(MODEL *model)
{
	return model->rsq;
}

qreal ViArmaInterpolator::calculateAR2(MODEL *model)
{
	return model->adjrsq;
}

qreal ViArmaInterpolator::calculateAIC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff;
}

qreal ViArmaInterpolator::calculateAICC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return (-2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff) + ((2 * model->ncoeff * (model->ncoeff + 1)) / qreal(model->nobs - model->ncoeff - 1));
}

qreal ViArmaInterpolator::calculateBIC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + model->ncoeff * log(model->nobs);
}

qreal ViArmaInterpolator::calculateHQC(MODEL *model)
{
	if(model->ess == DBL_MAX || model->ess <= 0.0 || model->ncoeff < 1 || model->nobs <= model->ncoeff)
	{
		return DBL_MAX;
	}
	return -2.0 * ((-0.5 * model->nobs * log(model->ess)) + (-0.5 * model->nobs * (LN2PI1 - log(qreal(model->nobs))))) + 2 * model->ncoeff * log(log(model->nobs));
}

ViArmaInterpolator* ViArmaInterpolator::clone()
{
	return new ViArmaInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViArmaInterpolator* create()
{
	return new ViArmaInterpolator();
}

#ifdef __cplusplus
}
#endif
*/
