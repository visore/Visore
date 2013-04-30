#include <vigaussianfunction.h>

#define EULERS_NUMBER 2.71828182845904523536

ViGaussianFunction::ViGaussianFunction()
{
	setHeight(1);
	setMean(0);
	setDeviation(1);
	mMinusTwoTimesDeviationSquared = 0;
}

ViGaussianFunction::ViGaussianFunction(qreal height, qreal center, qreal deviation)
{
	setHeight(height);
	setMean(center);
	setDeviation(deviation);
}

ViGaussianFunction::ViGaussianFunction(const ViGaussianFunction &other)
{
	mHeight = other.mHeight;
	mMean = other.mMean;
	mDeviation = other.mDeviation;
	mMinusTwoTimesDeviationSquared = other.mMinusTwoTimesDeviationSquared;
}

ViGaussianFunction::~ViGaussianFunction()
{
}
		
void ViGaussianFunction::setHeight(qreal height)
{
	mHeight = height;
}

void ViGaussianFunction::setMean(qreal center)
{
	mMean = center;
}

void ViGaussianFunction::setDeviation(qreal deviation)
{
	mDeviation = deviation;
	mMinusTwoTimesDeviationSquared = -2 * qPow(mDeviation, 2);
}

qreal ViGaussianFunction::height()
{
	return mHeight;
}

qreal ViGaussianFunction::mean()
{
	return mMean;
}

qreal ViGaussianFunction::deviation()
{
	return mDeviation;
}

qreal ViGaussianFunction::calculate(qreal x)
{
	return mHeight * qPow(EULERS_NUMBER, qPow((x - mMean), 2) / mMinusTwoTimesDeviationSquared);
}

ViGaussianDistribution::ViGaussianDistribution()
	: ViGaussianFunction()
{
}

ViGaussianDistribution::ViGaussianDistribution(qreal center, qreal deviation)
	: ViGaussianFunction(0, center, deviation)
{
}

ViGaussianDistribution::ViGaussianDistribution(const ViGaussianDistribution &other)
	: ViGaussianFunction(other)
{
}

void ViGaussianDistribution::setHeight(qreal height)
{
}

void ViGaussianDistribution::setDeviation(qreal deviation)
{
	ViGaussianFunction::setDeviation(deviation);
	ViGaussianFunction::setHeight(1 / (deviation * qSqrt(2 * M_PI)));
}
