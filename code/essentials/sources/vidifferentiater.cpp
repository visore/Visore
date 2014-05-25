#include <vidifferentiater.h>
#include <QtMath>

double ViDifferentiate::derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex, bool &error)
{
	// http://en.wikipedia.org/wiki/Finite_difference
	// http://en.wikipedia.org/wiki/Binomial_coefficient
	// http://oregonstate.edu/instruct/ch490/lessons/lesson11.htm

	static int required, halfRequired, index, i;
	static double value1, value2;

	required = derivativeIndex + 1;
	if(required % 2 == 0) ++required;
	halfRequired = required / 2;
	if(size < required || xIndex < halfRequired || (size - xIndex) < halfRequired)
	{
		error = true;
		return 0;
	}

	value1 = 0;
	for(i = 0; i <= derivativeIndex; ++i)
	{
		value2 = qPow(-1, i); // Positive or negative
		for(int j = 1; j <= i; ++j) value2 *= double(derivativeIndex + 1 - j) / j; // binomial coefficients
		if(derivativeIndex % 2 == 0) value2 *= points[xIndex + (derivativeIndex / 2) - i]; // Use exact point if x is an integer (eg: 0, 1, 2, 3, 4)
		else // If x is decimal (eg: 1.5) we use the average from the nearest 2 points (eg: 1 and 2)
		{
			index = xIndex + qFloor(derivativeIndex / 2.0) - i;
			value2 *= (points[index] + points[index + 1]) / 2.0;
		}
		value1 += value2;
	}
	error = false;
	return value1;
}

double ViDifferentiate::derivative(const int &derivativeIndex, const double *points, const int &xIndex)
{
	static int index, i;
	static double value1, value2;

	value1 = 0;
	for(i = 0; i <= derivativeIndex; ++i)
	{
		value2 = qPow(-1, i); // Positive or negative
		for(int j = 1; j <= i; ++j) value2 *= double(derivativeIndex + 1 - j) / j; // binomial coefficients
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
