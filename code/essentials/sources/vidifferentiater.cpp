#include <vidifferentiater.h>
#include <QtMath>

#include <vilogger.h>

inline double ViDifferentiater::calculate(const int &derivativeIndex, const double *points, const int &xIndex)
{
	// http://en.wikipedia.org/wiki/Finite_difference
	// http://en.wikipedia.org/wiki/Binomial_coefficient
	// http://oregonstate.edu/instruct/ch490/lessons/lesson11.htm

	static int index, i, j;
	static double value1, value2;

	value1 = 0;
	for(i = 0; i <= derivativeIndex; ++i)
	{
		value2 = qPow(-1, i); // Positive or negative
		for(j = 1; j <= i; ++j) value2 *= double(derivativeIndex + 1 - j) / j; // binomial coefficients
		if(derivativeIndex % 2 == 0) value2 *= points[xIndex + (derivativeIndex / 2) - i]; // Use exact point if x is an integer (eg: 0, 1, 2, 3, 4)
		else // If x is decimal (eg: 1.5) we use the average from the nearest 2 points (eg: 1 and 2)
		{
			index = xIndex + qFloor(derivativeIndex / 2.0) - i;
			value2 *= (points[index] + points[index + 1]) / 2.0;
		}
		value1 += value2;
	}
	return value1;
}

double ViDifferentiater::derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex, bool &error)
{
	static int required, halfRequired;

	required = derivativeIndex + 1;
	if(required % 2 == 0) ++required;
	halfRequired = required / 2;
	if(size < required || xIndex < halfRequired /* first samples */ || xIndex == size - 1 /* last samples */ || (size - xIndex) < halfRequired)
	{
		error = true;
		return 0;
	}

	error = false;
	return calculate(derivativeIndex, points, xIndex);
}

double ViDifferentiater::derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex)
{
	static int x;

	if(xIndex < derivativeIndex) x = xIndex + derivativeIndex; // first samples
	else if(xIndex >= size - derivativeIndex) x = xIndex - derivativeIndex; // last samples
	else x = xIndex;

	return calculate(derivativeIndex, points, x);
}

double ViDifferentiater::derivative(const int &derivativeIndex, const double *points, const int &xIndex)
{
	static int index, i, j;
	static double value1, value2;

	value1 = 0;
	for(i = 0; i <= derivativeIndex; ++i)
	{
		value2 = pow(-1, i); // Positive or negative
		for(j = 1; j <= i; ++j) value2 *= double(derivativeIndex + 1 - j) / j; // binomial coefficients
		if(derivativeIndex % 2 == 0) value2 *= points[xIndex + (derivativeIndex / 2) - i]; // Use exact point if x is an integer (eg: 0, 1, 2, 3, 4)
		else // If x is decimal (eg: 1.5) we use the average from the nearest 2 points (eg: 1 and 2)
		{
			index = xIndex + floor(derivativeIndex / 2.0) - i;
			value2 *= (points[index] + points[index + 1]) / 2.0;
		}
		value1 += value2;
	}
	return value1;
}

void ViDifferentiater::derivative(const int &derivativeIndex, const double *points, const int &size, double *derivatives)
{
	static int i, end;
	static qreal temp;

	// Calculte the know derivatives
	end = size - derivativeIndex;
	for(i = derivativeIndex; i < end; ++i) derivatives[i] = calculate(derivativeIndex, points, i);

	// Assume the derivative for the first points are equal to the first calculated derivative. Rough estimation.
	temp = derivatives[derivativeIndex];
	for(i = 0; i < derivativeIndex; ++i) derivatives[i] = temp;

	// Assume the derivative for the last points are equal to the last calculated derivative. Rough estimation.
	temp = derivatives[end - 1];
	for(i = end; i < size; ++i) derivatives[i] = temp;
}

void ViDifferentiater::derivative(const int &derivativeIndex, const double *pointsLeft, const int &sizeLeft, const double *pointsRight, const int &sizeRight, const int &sizeGap, double *derivatives)
{
	static int i, end, size;

	size = sizeLeft + sizeRight;
	qreal points[size];

	memcpy(points, pointsLeft, sizeof(double) * sizeLeft);
	memcpy(points + sizeLeft, pointsRight, sizeof(double) * sizeRight);
	derivative(derivativeIndex, points, size, derivatives);

	// Adjust the derivatives of the samples just before and after the gap
	for(i = sizeLeft - derivativeIndex; i < sizeLeft; ++i) derivatives[i] /= sizeGap;
	end = sizeLeft + derivativeIndex;
	for(i = sizeLeft; i < end; ++i) derivatives[i] /= sizeGap;

	// This one doesn't assume there is a gradient between last-left and first-right, but treats left and right like 2 seperate (un-correlated) problems
	/*qreal temp = derivatives[sizeLeft - derivativeIndex-1];
	for(i = sizeLeft - derivativeIndex; i < sizeLeft; ++i) derivatives[i] =temp;
	end = sizeLeft + derivativeIndex;
	temp = derivatives[end];
	for(i = sizeLeft; i < end; ++i) derivatives[i] =temp;*/
}

void ViDifferentiater::derivative(const int &derivativeIndex, const double *pointsLeft, const int &sizeLeft, const double *pointsRight, const int &sizeRight, const int &sizeGap, double *derivativesLeft, double *derivativesRight)
{
	static int i;

	qreal derivatives[sizeLeft + sizeRight];
	derivative(derivativeIndex, pointsLeft, sizeLeft, pointsRight, sizeRight, sizeGap, derivatives);

	for(i = 0; i < sizeLeft; ++i) derivativesLeft[i] = derivatives[i];
	for(i = 0; i < sizeRight; ++i) derivativesRight[i] = derivatives[i + sizeLeft];
}
