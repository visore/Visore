#include "vicompareresult.h"

ViCompareResult::ViCompareResult()
{
	reset();
}

void ViCompareResult::setNumberOfSamples(qint64 numberOfSamples)
{
	mNumberOfSamples = numberOfSamples;
}

void ViCompareResult::setNumberOfMatchingSamples(qint64 numberOfMatchingSamples)
{
	mNumberOfMatchingSamples = numberOfMatchingSamples;
}

void ViCompareResult::setNumberOfNoneMatchingSamples(qint64 numberOfNoneMatchingSamples)
{
	mNumberOfNoneMatchingSamples = numberOfNoneMatchingSamples;
}

void ViCompareResult::setStandardDeviation(qreal standardDeviation)
{
	mStandardDeviation = standardDeviation;
}

void ViCompareResult::setAverageDifference(qreal averageDifference)
{
	mAverageDifference = averageDifference;
}

void ViCompareResult::setMaximumDifference(qreal maximumDifference)
{
	mMaximumDifference = maximumDifference;
}

void ViCompareResult::setMinimumDifference(qreal minimumDifference)
{
	mMinimumDifference = minimumDifference;
}

qint64 ViCompareResult::numberOfSamples()
{
	return mNumberOfSamples;
}

qint64 ViCompareResult::numberOfMatchingSamples()
{
	return mNumberOfMatchingSamples;
}

qint64 ViCompareResult::numberOfNoneMatchingSamples()
{
	return mNumberOfNoneMatchingSamples;
}

qreal ViCompareResult::standardDeviation()
{
	return mStandardDeviation;
}

qreal ViCompareResult::averageDifference()
{
	return mAverageDifference;
}

qreal ViCompareResult::maximumDifference()
{
	return mMaximumDifference;
}

qreal ViCompareResult::minimumDifference()
{
	return mMinimumDifference;
}

void ViCompareResult::reset()
{
	mNumberOfSamples = 0;
	mNumberOfMatchingSamples = 0;
	mNumberOfNoneMatchingSamples = 0;
	mStandardDeviation = 0;
	mAverageDifference = 0;
	mMaximumDifference = 0;
	mMinimumDifference = 0;
}

QString ViCompareResult::toString()
{
	QString result = "";

	result += "Number of samples: " + QString::number(mNumberOfSamples) + "\n";
	result += "Number of matching samples: " + QString::number(mNumberOfMatchingSamples) + "\n";
	result += "Number of none-matching samples: " + QString::number(mNumberOfNoneMatchingSamples) + "\n";

	result += "Standard deviation: " + QString::number(mStandardDeviation, 'f', 8) + "\n";
	result += "Average difference: " + QString::number(mAverageDifference, 'f', 8) + "\n";
	result += "Maximum difference: " + QString::number(mMaximumDifference, 'f', 8) + "\n";
	result += "Minimum difference: " + QString::number(mMinimumDifference, 'f', 8) + "\n";

	return result;
}
