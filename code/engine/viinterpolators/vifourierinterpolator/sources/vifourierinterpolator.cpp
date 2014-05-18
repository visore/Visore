#include <vifourierinterpolator.h>
#include <viscaler.h>
#include <float.h>

#include<vilogger.h>

#define TWO_PI 6.283185307179586476925286766559

#define MAX_TEST_DEGREE 10

#define DEFAULT_DEGREE 3
#define DEFAULT_SELECTION Best

ViFourierInterpolator::ViFourierInterpolator()
	: ViDegreeInterpolator(DEFAULT_DEGREE)
{
	setOrderSelection(DEFAULT_SELECTION);
}

ViFourierInterpolator::ViFourierInterpolator(const int &degree)
	: ViDegreeInterpolator(degree)
{
	setOrderSelection(DEFAULT_SELECTION);
}

void ViFourierInterpolator::setOrderSelection(OrderSelection selection)
{
	if(selection == Fixed) interpolatePointer = &ViFourierInterpolator::interpolateFixed;
	else if(selection == Best) interpolatePointer = &ViFourierInterpolator::interpolateBest;
}

void ViFourierInterpolator::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViFourierInterpolator::setParameters(const qreal &param1, const qreal &param2)
{
	setWindowSize(param1);
	setDegree(param2);
}

bool ViFourierInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf
	// mathcs.slu.edu/Public/johnson/maths/interpolation.pdf
	// https://www.math.wisc.edu/~robbin/542dir/VectorSpaces.pdf

	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
}

bool ViFourierInterpolator::interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	ViVector coefficients;
	if(estimate(mDegree, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize))
	{
		interpolate(mDegree, coefficients, leftSize, rightSize, outputSamples, outputSize);
		return true;
	}
	return false;
}

bool ViFourierInterpolator::interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	ViVector coefficients, bestCoefficients;
	qreal currentEss, bestEss = DBL_MAX;
	int bestDegree = 1, currentDegree = 1;

	while(true)
	{
		if(estimate(currentDegree, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize))
		{
			currentEss = ess(currentDegree, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize);
			if(currentEss < bestEss)
			{
				bestEss = currentEss;
				bestDegree = currentDegree;
				bestCoefficients = coefficients;
			}
			else break;
			++currentDegree;
		}
	}
//cout<<bestDegree<<"\t"<<bestEss<<endl;
	interpolate(bestDegree, bestCoefficients, leftSize, rightSize, outputSamples, outputSize);
	return true;



	/*static int i;

	ViVector coefficients, bestCoefficients;
	qreal currentEss, bestEss = DBL_MAX;
	int bestDegree = 1;

	for(i = 1; i <= 15; ++i)
	{
		if(estimate(i, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize))
		{
			currentEss = ess(i, coefficients, leftSamples, leftSize, rightSamples, rightSize, outputSize);
			if(currentEss < bestEss)
			{
				bestEss = currentEss;
				bestDegree = i;
				bestCoefficients = coefficients;
			}
		}
	}

	interpolate(bestDegree, bestCoefficients, leftSize, rightSize, outputSamples, outputSize);
	return true;*/
}

bool ViFourierInterpolator::estimate(const int &degree, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize)
{
	static int i, j, sampleCount, coefficientCount, rightOffset, index;
	static qreal multiplier, value1, value2;

	coefficientCount = (2 * degree) + 1;
	sampleCount = leftSize + rightSize;
	rightOffset = leftSize + outputSize;

	ViMatrix matrix(sampleCount, coefficientCount);
	ViVector vector(sampleCount);

	multiplier = TWO_PI / (sampleCount + outputSize);

	for(i = 0; i < leftSize; ++i) vector[i] = leftSamples[i];
	for(i = 0; i < rightSize; ++i) vector[i + leftSize] = rightSamples[i];

	for(i = 0; i < leftSize; ++i)
	{
		value1 = multiplier * i;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[i][j] = qCos(value2);
			matrix[i][j + degree] = qSin(value2);
		}
	}
	for(i = 0; i < rightSize; ++i)
	{
		index = i + leftSize;
		value1 = multiplier * (i + rightOffset);
		matrix[index][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[index][j] = qCos(value2);
			matrix[index][j + degree] = qSin(value2);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierInterpolator::interpolate(const int &degree, const ViVector &coefficients, const int &leftSize, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j;
	static qreal value, value1, value2, multiplier;

	multiplier = TWO_PI / (leftSize + rightSize + outputSize);

	for(i = 0; i < outputSize; ++i)
	{
		value1 = multiplier * (leftSize + i);
		value = coefficients[0] / 2;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			value += coefficients[j] * qCos(value2);
			value += coefficients[j + degree] * qSin(value2);
		}
		outputSamples[i] = value;
	}
}

qreal ViFourierInterpolator::ess(const int &degree, const ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize)
{
	static int i, j, rightOffset;
	static qreal value, value1, value2, multiplier, error;

	rightOffset = leftSize + outputSize;
	multiplier = TWO_PI / (leftSize + rightSize + outputSize);
	error = 0;

	for(i = 0; i < leftSize; ++i)
	{
		value1 = multiplier * i;
		value = coefficients[0] / 2;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			value += coefficients[j] * qCos(value2);
			value += coefficients[j + degree] * qSin(value2);
		}
		error += qPow(value - leftSamples[i], 2);
	}

	for(i = 0; i < rightSize; ++i)
	{
		value1 = multiplier * (i + rightOffset);
		value = coefficients[0] / 2;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			value += coefficients[j] * qCos(value2);
			value += coefficients[j + degree] * qSin(value2);
		}
		error += qPow(value - rightSamples[i], 2);
	}

	return error;
}

ViFourierInterpolator* ViFourierInterpolator::clone()
{
	return new ViFourierInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFourierInterpolator* create()
{
	return new ViFourierInterpolator();
}

#ifdef __cplusplus
}
#endif
