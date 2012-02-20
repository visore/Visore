#include "viamplitude.h"

ViAmplitude::ViAmplitude(double maximum, double minimum, double maximumAverage, double minimumAverage)
{
	mMaximum = maximum;
	mMinimum = minimum;
	mMaximumAverage = maximumAverage;
	mMinimumAverage = minimumAverage;
}

double ViAmplitude::maximum()
{
	return mMaximum;
}

double ViAmplitude::minimum()
{
	return mMinimum;
}

double ViAmplitude::maximumAverage()
{
	return mMaximumAverage;
}

double ViAmplitude::minimumAverage()
{
	return mMinimumAverage;
}
