#include <vinewtonpredictor.h>
#include <vilogger.h>

class ViNewtonPredictor::ViNewtonTypedBase
{

	public:

		virtual bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViEigenBase *eigen, ViError *error = NULL) = 0;
		virtual void solve(const ViEigenBaseVector *coefficients, const qreal *x, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling, ViEigenBase *eigen) = 0;

};

template <typename T>
class ViNewtonTyped : public ViNewtonPredictor::ViNewtonTypedBase
{

	public:

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViEigenBase *eigen, ViError *error = NULL)
		{
			// en.wikipedia.org/wiki/Newton_polynomial#Main_idea

			static int i, j, k, offset;
			static qreal scaling;

			scaling = (size + predictionCount - 1);

			qreal x[size];

			ViEigenBaseMatrix *matrix = eigen->createMatrix(size, size);
			ViEigenBaseVector *vector = eigen->createVector(size);
			T product;
			eigen->intialize(product);

			for(i = 0; i < size; ++i)
			{
				x[i] = i / scaling;
				vector->set(i, samples[i]);
			}

			for(i = 0; i < size; ++i)
			{
				matrix->set(i, 0, 1);
				for(j = 1; j < size; ++j)
				{
					product = 1;
					for(k = 0; k < j; ++k) product *= x[i] - x[k];
					matrix->set(i, j, product);
				}
			}

			ViEigenBaseVector *coefficients = eigen->estimate(matrix, vector);
			eigen->clear(matrix);
			eigen->clear(vector);

			solve(coefficients, x, size, predictedSamples, predictionCount, size, scaling, eigen);

			if(error != NULL)
			{
				qreal model[size];
				solve(coefficients, x, size, model, size, 0, scaling, eigen);
				error->add(model, samples, size);
			}

			eigen->clear(coefficients);
		}

		void solve(const ViEigenBaseVector *coefficients, const qreal *x, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling, ViEigenBase *eigen)
		{
			static int i, j, k;
			static qreal scaledX;

			ViEigenBaseMatrix *matrix = eigen->createMatrix(outputSize, size);
			T product;
			eigen->intialize(product);

			for(i = 0; i < outputSize; ++i)
			{
				scaledX = (i + startX) / scaling;
				matrix->set(i, 0, 1);
				for(j = 1; j < size; ++j)
				{
					product = 1;
					for(k = 0; k < j; ++k) product *= scaledX - x[k];
					matrix->set(i, j, product);
				}
			}
			eigen->solve(coefficients, matrix, output, outputSize);
			eigen->clear(matrix);
		}

};


ViNewtonPredictor::ViNewtonPredictor()
	: ViPredictor()
{
	mEigen = NULL;
	mTyped = NULL;
	addParameterName("Window Size");
}

ViNewtonPredictor::ViNewtonPredictor(const ViNewtonPredictor &other)
	: ViPredictor(other)
{
	mEigen = other.mEigen;
	mTyped = other.mTyped;
}

ViNewtonPredictor::~ViNewtonPredictor()
{
	delete mTyped;
	mTyped = NULL;
}

void ViNewtonPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
	setType();
}

void ViNewtonPredictor::setType()
{
	delete mTyped;
	if(mWindowSize < 16)
	{
		mTyped = new ViNewtonTyped<double>();
		mEigen = ViEigenManager::getByBits(53);
	}
	else
	{
		mTyped = new ViNewtonTyped<mpfr::mpreal>();
		if(mWindowSize < 22) mEigen = ViEigenManager::getByBits(64);
		else if(mWindowSize < 24) mEigen = ViEigenManager::getByBits(72);
		else if(mWindowSize < 26) mEigen = ViEigenManager::getByBits(80);
		else if(mWindowSize < 28) mEigen = ViEigenManager::getByBits(88);
		else if(mWindowSize < 30) mEigen = ViEigenManager::getByBits(96);
		else if(mWindowSize < 32) mEigen = ViEigenManager::getByBits(104);
		else if(mWindowSize < 34) mEigen = ViEigenManager::getByBits(112);
		else if(mWindowSize < 36) mEigen = ViEigenManager::getByBits(120);
		else if(mWindowSize < 38) mEigen = ViEigenManager::getByBits(128);
		else if(mWindowSize < 42) mEigen = ViEigenManager::getByBits(144);
		else if(mWindowSize < 46) mEigen = ViEigenManager::getByBits(160);
		else if(mWindowSize < 50) mEigen = ViEigenManager::getByBits(176);
		else if(mWindowSize < 54) mEigen = ViEigenManager::getByBits(192);
		else if(mWindowSize < 58) mEigen = ViEigenManager::getByBits(208);
		else if(mWindowSize < 62) mEigen = ViEigenManager::getByBits(224);
		else if(mWindowSize < 66) mEigen = ViEigenManager::getByBits(240);
		else if(mWindowSize < 70) mEigen = ViEigenManager::getByBits(256);
		else if(mWindowSize < 78) mEigen = ViEigenManager::getByBits(288);
		else if(mWindowSize < 86) mEigen = ViEigenManager::getByBits(320);
		else if(mWindowSize < 94) mEigen = ViEigenManager::getByBits(352);
		else if(mWindowSize < 102) mEigen = ViEigenManager::getByBits(384);
		else if(mWindowSize < 110) mEigen = ViEigenManager::getByBits(416);
		else if(mWindowSize < 118) mEigen = ViEigenManager::getByBits(448);
		else if(mWindowSize < 126) mEigen = ViEigenManager::getByBits(480);
		else if(mWindowSize < 134) mEigen = ViEigenManager::getByBits(512);
		else if(mWindowSize < 150) mEigen = ViEigenManager::getByBits(640);
		else if(mWindowSize < 166) mEigen = ViEigenManager::getByBits(768);
		else if(mWindowSize < 182) mEigen = ViEigenManager::getByBits(896);
		else mEigen = ViEigenManager::getByBits(1024);
	}
}

bool ViNewtonPredictor::validParameters()
{
	return mWindowSize > 0;
}

bool ViNewtonPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error)
{
	mTyped->predict(samples, size, predictedSamples, predictionCount, mEigen, error);
	return true;
}

ViNewtonPredictor* ViNewtonPredictor::clone()
{
	return new ViNewtonPredictor(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViNewtonPredictor* create()
{
	return new ViNewtonPredictor();
}

#ifdef __cplusplus
}
#endif


/*#include <vinewtonpredictor.h>
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

bool ViNewtonPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViErrorCollection *modelErrors)
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
*/
