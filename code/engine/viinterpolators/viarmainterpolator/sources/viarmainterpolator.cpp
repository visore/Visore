#include <viarmainterpolator.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 128
#define MAXIMUM_ITERATIONS 10

#define MAXIMUM_TEST_DEGREE 30

ViArmaInterpolator::ViArmaInterpolator()
	: ViAutoDegreeInterpolator(DEFAULT_WINDOW_SIZE*2)
{
	mGretlParameters = NULL;
	mGretlData = NULL;
	mWindowSize = DEFAULT_WINDOW_SIZE;
	mPacfConfidenceLevel = 1.96 / qSqrt(mWindowSize - 1);
	initialize();
}

ViArmaInterpolator::ViArmaInterpolator(const int &degree)
	: ViAutoDegreeInterpolator(degree)
{
	mGretlParameters = NULL;
	mGretlData = NULL;
	initialize();
}

ViArmaInterpolator::~ViArmaInterpolator()
{
	clear();
}

void ViArmaInterpolator::initialize()
{
	ViGretl::initialize();

	libset_set_int(BFGS_MAXITER, MAXIMUM_ITERATIONS); // ML
	libset_set_int(BHHH_MAXITER, MAXIMUM_ITERATIONS); // CML

	mGretlParameters = gretl_list_new(5);
	mGretlParameters[1] = 2;        // AR order
	mGretlParameters[2] = 0;        // order of integration
	mGretlParameters[3] = 0;        // MA order
	mGretlParameters[4] = LISTSEP;  // separator
	mGretlParameters[5] = 1;

	mGretlData = create_new_dataset(2, DEFAULT_WINDOW_SIZE + 1, 0); // + 1 for out-of-sample prediction
	mGretlData->t2 = mGretlData->n - 2; // reserve 1 observations for out-of-sample forecasting
}

void ViArmaInterpolator::clear()
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

bool ViArmaInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static bool failed;
	static int i, error, count, end;
	static qreal prediction;

	if(leftSize-1 != DEFAULT_WINDOW_SIZE || rightSize-1 != DEFAULT_WINDOW_SIZE)
	{
		LOG("Not enough data: " + QString::number(leftSize) + ".", QtCriticalMsg);
		exit(-1);
	}

	count = 0;
	failed = false;
	while(count < outputSize)
	{
		end = leftSize - count;

		for(i = 0; i < end; ++i) mGretlData->Z[1][i] = leftSamples[i + count + 1]; // Add real data
		for(i = 0; i < count; ++i) mGretlData->Z[1][end + i] = outputSamples[i]; // Add predicted values

		MODEL *model = gretl_model_new();
		*model = arma_model(mGretlParameters, NULL, mGretlData, OPT_C, NULL);
		//gretlAutocorrelationModel(model);
		//gretlFixedModel(model);

		if(model->errcode) failed = true;
		else
		{
			FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
			if(error) failed = true;
			else
			{
				if(forecast->nobs > DEFAULT_WINDOW_SIZE) prediction = forecast->fitted[DEFAULT_WINDOW_SIZE];
				free_fit_resid(forecast);
			}
		}
		gretl_model_free(model);

		if(failed)
		{
			for(i = 0; i < end; ++i) prediction += leftSamples[i + count + 1];
			for(i = 0; i < count; ++i) prediction += outputSamples[i];
			outputSamples[count] += prediction / leftSize;
		}
		else
		{
			outputSamples[count] += prediction;
		}

		/*********************************/

		/*end = rightSize - count;

		for(i = 0; i < end; ++i) mGretlData->Z[1][i] = rightSamples[rightSize - i - count]; // Add real data
		for(i = 0; i < count; ++i) mGretlData->Z[1][end + i] = outputSamples[outputSize - i - 1]; // Add predicted values

		model = gretl_model_new();
		*model = arma_model(mGretlParameters, NULL, mGretlData, OPT_C, NULL);
		//gretlAutocorrelationModel(model);
		//gretlFixedModel(model);

		if(model->errcode) failed = true;
		else
		{
			FITRESID *forecast = get_forecast(model, mGretlData->t2 + 1, mGretlData->n - 1, 0, mGretlData, OPT_NONE, &error);
			if(error) failed = true;
			else
			{
				if(forecast->nobs > DEFAULT_WINDOW_SIZE) prediction = forecast->fitted[DEFAULT_WINDOW_SIZE];
				free_fit_resid(forecast);
			}
		}
		gretl_model_free(model);

		if(failed)
		{
			for(i = 0; i < end; ++i) prediction += rightSamples[rightSize - i - count];
			for(i = 0; i < count; ++i) prediction += outputSamples[outputSize - i - 1];
			outputSamples[count] += prediction / rightSize;
		}
		else
		{
			outputSamples[count] += prediction;
		}*/

		/*********************************/

		++count;
	}

	//for(i = 0; i < outputSize; ++i) outputSamples[i] /= 2;
}

void ViArmaInterpolator::gretlFixedModel(MODEL *model)
{
	mGretlParameters[1] = 15;//mArDegree;
	mGretlParameters[3] = 0;//mMaDegree;
	while(true)
	{
		*model = arma_model(mGretlParameters, NULL, mGretlData, OPT_NONE/*(gretlopt) mGretlEstimation*/, NULL);
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
}

void ViArmaInterpolator::gretlAutocorrelationModel(MODEL *model)
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
		*model = arma_model(mGretlParameters, NULL, mGretlData, OPT_NONE/*(gretlopt) mGretlEstimation*/, NULL);
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
