#include <vinewtonpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>
#include <vidifferentiater.h>
#include <float.h>

#define DEFAULT_DEGREE 3

ViNewtonPredictor::ViNewtonPredictor(const Estimation &estimation)
	: ViPredictor()
{
	setPointers(estimation);
	setDegree(DEFAULT_DEGREE);

	addParameterName("Window Size");
	addParameterName("Degree");
}

ViNewtonPredictor::ViNewtonPredictor(const ViNewtonPredictor &other)
	: ViPredictor(other)
{
	setPointers(other.mEstimation);
	setDegree(other.mDegree);
}

ViNewtonPredictor::~ViNewtonPredictor()
{
}

QString ViNewtonPredictor::name(QString replace, bool spaced)
{
	QString estimation;

	if(mEstimation == Fixed) estimation = "Fixed Estimation";
	else if(mEstimation == Best) estimation = "Best Estimation";

	QString name = ViPredictor::name(replace, spaced) + " (" + estimation + ")";
	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViNewtonPredictor::setDegree(const int &degree)
{
	mDegree = degree;

	mBestParameters.clear();
	mBestParameters.resize(mDegree + 1);
	mBestParameters.fill(0);
}

void ViNewtonPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViNewtonPredictor::validParameters()
{
	return validParameters(mWindowSize, mDegree);
}

bool ViNewtonPredictor::validParameters(const int &windowSize, const int &degree)
{
	return windowSize >= degree + 1;
}

void ViNewtonPredictor::setPointers(const Estimation &estimation)
{
	mEstimation = estimation;
	if(mEstimation == Fixed) predictPointer = &ViNewtonPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViNewtonPredictor::predictBest;
}

bool ViNewtonPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// Scaling because multiplying integers (or powers) might lead to variable overflow
	return (this->*predictPointer)(samples, size, predictedSamples, predictionCount, qreal(predictionCount + size - 1));
}

bool ViNewtonPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	if(validParameters(size, mDegree))
	{
		static int i;
		static ViVector coefficients;
		if(estimateModel(mDegree, coefficients, samples, size, scaling)) predictModel(mDegree, coefficients, predictedSamples, predictionCount, size, scaling);
		else for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
		return true;
	}
	LOG("Invalid parameter combination detected.", QtCriticalMsg);
	return false;
}

bool ViNewtonPredictor::predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	static int i;
	static ViVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;

	qreal prediction[size];
	bestMse = DBL_MAX;

	for(i = 1; i <= mDegree; ++i)
	{
		if(validParameters(size, i))
		{
			if(estimateModel(i, currentCoefficients, samples, size, scaling))
			{
				predictModel(i, currentCoefficients, prediction, size, 0, scaling);
				currentMse = calculateMse(samples, prediction, size);
				if(currentMse < bestMse)
				{
					bestMse = currentMse;
					bestCoefficients = currentCoefficients;
				}
				else break;
			}
		}
	}
	if(bestMse == DBL_MAX) return false;

	--i; // Important: last iteration will increase i before exiting the loop
	mBestParameters[i] += 1;
	predictModel(i, bestCoefficients, predictedSamples, predictionCount, size, scaling);
	return true;
}

bool ViNewtonPredictor::estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, k;
	static qreal value, x;

	ViMatrix matrix(size, degree + 1);
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		x = i / scaling;
		vector[i] = samples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= degree; ++j)
		{
			value = 1;
			for(k = 0; k < j; ++k) value *= x - (k / scaling);
			matrix[i][j] = value;
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViNewtonPredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j, k;
	static qreal value, result, x;

	for(i = 0; i < size; ++i)
	{
		x = (start + i) / scaling;
		result = coefficients[0];
		for(j = 1; j <= degree; ++j)
		{
			value = 1;
			for(k = 0; k < j; ++k) value *= x - (k / scaling);
			result += coefficients[j] * value;
		}
		prediction[i] = result;
	}
}

qreal ViNewtonPredictor::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}
