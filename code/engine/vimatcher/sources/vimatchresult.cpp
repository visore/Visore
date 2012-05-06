#include "vimatchresult.h"

ViMatchResultCombination::ViMatchResultCombination(qreal minimum, qreal maximum, qreal average)
{
	mMinimum = minimum;
	mMaximum = maximum;
	mAverage = average;
}

void ViMatchResultCombination::setMinimum(qreal minimum)
{
	mMinimum = minimum;
}

void ViMatchResultCombination::setMaximum(qreal maximum)
{
	mMaximum = maximum;
}

void ViMatchResultCombination::setAverage(qreal average)
{
	mAverage = average;
}

qreal ViMatchResultCombination::minimum()
{
	return mMinimum;
}

qreal ViMatchResultCombination::maximum()
{
	return mMaximum;
}

qreal ViMatchResultCombination::average()
{
	return mAverage;
}

QString ViMatchResultCombination::toString()
{
	return "(min: " + QString::number(mMinimum, 'f', 8) + ", max: " + QString::number(mMaximum, 'f', 8) + ", avg: " + QString::number(mAverage, 'f', 8) + ")";
}

ViMatchResult::ViMatchResult()
{
	reset();
}

void ViMatchResult::reset()
{
	mCrossCorrelation = ViMatchResultCombination();
	mSampleDifference = ViMatchResultCombination();
}

QString ViMatchResult::toString()
{
	QString string = "";
	string += "Sample difference: " + mSampleDifference.toString() + "\n";
	string += "Cross correlation: " + mCrossCorrelation.toString();
	return string;
}

void ViMatchResult::setCrossCorrelation(ViMatchResultCombination combination)
{
	mCrossCorrelation = combination;
}

void ViMatchResult::setSampleDifference(ViMatchResultCombination combination)
{
	mSampleDifference = combination;
}

qreal ViMatchResult::crossCorrelation(ResultType type)
{
	if(type == ViMatchResult::Maximum)
	{
		return mCrossCorrelation.maximum();
	}
	if(type == ViMatchResult::Minimum)
	{
		return mCrossCorrelation.minimum();
	}
	if(type == ViMatchResult::Average)
	{
		return mCrossCorrelation.average();
	}
}

qreal ViMatchResult::sampleDifference(ResultType type)
{
	if(type == ViMatchResult::Maximum)
	{
		return mSampleDifference.maximum();
	}
	if(type == ViMatchResult::Minimum)
	{
		return mSampleDifference.minimum();
	}
	if(type == ViMatchResult::Average)
	{
		return mSampleDifference.average();
	}
}
