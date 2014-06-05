#include <vinewtoninterpolator.h>
#include <vieigen.h>
#include <vilogger.h>

ViNewtonInterpolator::ViNewtonInterpolator()
	: ViInterpolator()
{
	//ViEigen::initialize();

	ViEigen<55> e;

	ViEigen<55>::Matrix m = e.createMatrix();

	addParameterName("Window Size");
}

ViNewtonInterpolator::ViNewtonInterpolator(const ViNewtonInterpolator &other)
	: ViInterpolator(other)
{
}

ViNewtonInterpolator::~ViNewtonInterpolator()
{
}

void ViNewtonInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
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
	// en.wikipedia.org/wiki/Newton_polynomial#Main_idea

	/*static int i, j, k, size, offset;
	static qreal scaledX, scaling, product;
	static bigreal result;

	size = leftSize + rightSize;
	scaling = (size + outputSize - 1);

	qreal x[size];

	ViEigenBigMatrix matrix(size, size);
	ViEigenBigVector vector(size);

	for(i = 0; i < leftSize; ++i)
	{
		x[i] = i / scaling;
		vector[i] = leftSamples[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		offset = i + leftSize;
		x[offset] = (offset + outputSize) / scaling;
		vector(offset) = rightSamples[i];
	}

	for(i = 0; i < size; ++i)
	{
		matrix(i, 0) = 1;
		for(j = 1; j < size; ++j)
		{
			product = 1;
			for(k = 0; k < j; ++k) product *= x[i] - x[k];
			matrix(i, j) = product;
		}
	}

	ViEigenBigVector coefficients = ViEigen::solve(matrix, vector);
*/
	/*for(i = 0; i < size; ++i)
	{
		for(j = 0; j < size; ++j)
		{
			cout<<matrix(i,j)<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;
	for(i = 0; i < size; ++i)
	{
		cout<<coefficients(i)<<" ";
	}
	cout<<endl;
	exit(-1);*/

	/*for(i = 0; i < outputSize; ++i)
	{
		scaledX = (i + leftSize) / scaling;
		result = coefficients(0);
		for(j = 1; j < size; ++j)
		{
			product = 1;
			for(k = 0; k < j; ++k) product *= scaledX - x[k];
			result += coefficients(j) * product;
		}
		outputSamples[i] = ViEigen::toReal(result);
	}

	return true;*/
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
