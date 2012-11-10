#include "vicorrelation.h"
#include "vicorrelator.h"

ViCorrelation::ViCorrelation(qreal worst, qreal best, qreal average)
{
	mWorst = worst;
	mBest = best;
	mAverage = average;
}

ViCorrelation::ViCorrelation(const ViCorrelation &other)
{
	mWorst = other.mWorst;
	mBest = other.mBest;
	mAverage = other.mAverage;
}

void ViCorrelation::setWorst(qreal worst)
{
	mWorst = worst;
}

void ViCorrelation::setBest(qreal best)
{
	mBest = best;
}

void ViCorrelation::setAverage(qreal average)
{
	mAverage = average;
}

qreal ViCorrelation::worst()
{
	return mWorst;
}

qreal ViCorrelation::best()
{
	return mBest;
}

qreal ViCorrelation::average()
{
	return mAverage;
}

QString ViCorrelation::toString()
{
	return "(worst: " + QString::number(mWorst, 'f', 8) + ", best: " + QString::number(mBest, 'f', 8) + ", avg: " + QString::number(mAverage, 'f', 8) + ")";
}
