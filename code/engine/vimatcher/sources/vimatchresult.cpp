#include "vimatchresult.h"

ViMatchResultCombination::ViMatchResultCombination(qreal worst, qreal best, qreal average)
{
	mWorst = worst;
	mBest = best;
	mAverage = average;
}

void ViMatchResultCombination::setWorst(qreal worst)
{
	mWorst = worst;
}

void ViMatchResultCombination::setBest(qreal best)
{
	mBest = best;
}

void ViMatchResultCombination::setAverage(qreal average)
{
	mAverage = average;
}

qreal ViMatchResultCombination::worst()
{
	return mWorst;
}

qreal ViMatchResultCombination::best()
{
	return mBest;
}

qreal ViMatchResultCombination::average()
{
	return mAverage;
}

QString ViMatchResultCombination::toString()
{
	return "(worst: " + QString::number(mWorst, 'f', 8) + ", best: " + QString::number(mBest, 'f', 8) + ", avg: " + QString::number(mAverage, 'f', 8) + ")";
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
	string += "Sample-correlation: " + mSampleDifference.toString() + "\n";
	string += "Cross-correlation: " + mCrossCorrelation.toString();
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
	if(type == ViMatchResult::Best)
	{
		return mCrossCorrelation.best();
	}
	if(type == ViMatchResult::Worst)
	{
		return mCrossCorrelation.worst();
	}
	if(type == ViMatchResult::Average)
	{
		return mCrossCorrelation.average();
	}
}

qreal ViMatchResult::sampleDifference(ResultType type)
{
	if(type == ViMatchResult::Best)
	{
		return mSampleDifference.best();
	}
	if(type == ViMatchResult::Worst)
	{
		return mSampleDifference.worst();
	}
	if(type == ViMatchResult::Average)
	{
		return mSampleDifference.average();
	}
}
