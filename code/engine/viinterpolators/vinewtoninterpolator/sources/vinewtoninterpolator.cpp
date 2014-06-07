#include <vinewtoninterpolator.h>
#include <vilogger.h>


class ViNewtonInterpolator::ViNewtonTypedBase
{

	public:

		virtual void interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViEigenBase *eigen) = 0;

};

template <typename T>
class ViNewtonTyped : public ViNewtonInterpolator::ViNewtonTypedBase
{

	public:

		void interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViEigenBase *eigen)
		{
			// en.wikipedia.org/wiki/Newton_polynomial#Main_idea

			static int i, j, k, size, offset;
			static qreal scaledX, scaling;

			size = leftSize + rightSize;
			scaling = (size + outputSize - 1);

			qreal x[size];

			ViEigenBaseMatrix *matrix = eigen->createMatrix(size, size);
			ViEigenBaseVector *vector = eigen->createVector(size);
			T product;
			eigen->intialize(product);

			for(i = 0; i < leftSize; ++i)
			{
				x[i] = i / scaling;
				vector->set(i, leftSamples[i]);
			}
			for(i = 0; i < rightSize; ++i)
			{
				offset = i + leftSize;
				x[offset] = (offset + outputSize) / scaling;
				vector->set(offset, rightSamples[i]);
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

			matrix = eigen->createMatrix(outputSize, size);

			for(i = 0; i < outputSize; ++i)
			{
				scaledX = (i + leftSize) / scaling;
				matrix->set(i, 0, 1);
				for(j = 1; j < size; ++j)
				{
					product = 1;
					for(k = 0; k < j; ++k) product *= scaledX - x[k];
					matrix->set(i, j, product);
				}
			}

			eigen->solve(coefficients, matrix, outputSamples, outputSize);
			eigen->clear(matrix);
			eigen->clear(coefficients);
		}

};


ViNewtonInterpolator::ViNewtonInterpolator()
	: ViInterpolator()
{
	mEigen = NULL;
	mTyped = NULL;
	addParameterName("Window Size");
}

ViNewtonInterpolator::ViNewtonInterpolator(const ViNewtonInterpolator &other)
	: ViInterpolator(other)
{
	mEigen = other.mEigen;
	mTyped = other.mTyped;
}

ViNewtonInterpolator::~ViNewtonInterpolator()
{
	delete mTyped;
	mTyped = NULL;
}

void ViNewtonInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}

	mEigen = ViEigenManager::getByBits(128);
	delete mTyped;
	mTyped = new ViNewtonTyped<mpfr::mpreal>();
}

bool ViNewtonInterpolator::validParameters()
{
	return validParameters(mWindowSize);
}

bool ViNewtonInterpolator::validParameters(const int &windowSize)
{
	return windowSize > 1 && windowSize % 2 == 0;
}

bool ViNewtonInterpolator::validParameters(const int &leftSize, const int &rightSize)
{
	return leftSize != 0 && rightSize != 0 && validParameters(leftSize + rightSize);
}

bool ViNewtonInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	mTyped->interpolate(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize, mEigen);
	return true;
}

ViNewtonInterpolator* ViNewtonInterpolator::clone()
{
	return new ViNewtonInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViNewtonInterpolator* create()
{
	return new ViNewtonInterpolator();
}

#ifdef __cplusplus
}
#endif
