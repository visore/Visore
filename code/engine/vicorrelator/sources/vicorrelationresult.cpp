#include "vicorrelationresult.h"

ViCorrelationResultCombination::ViCorrelationResultCombination(qreal worst, qreal best, qreal average)
{
	mWorst = worst;
	mBest = best;
	mAverage = average;
}

void ViCorrelationResultCombination::setWorst(qreal worst)
{
	mWorst = worst;
}

void ViCorrelationResultCombination::setBest(qreal best)
{
	mBest = best;
}

void ViCorrelationResultCombination::setAverage(qreal average)
{
	mAverage = average;
}

qreal ViCorrelationResultCombination::worst()
{
	return mWorst;
}

qreal ViCorrelationResultCombination::best()
{
	return mBest;
}

qreal ViCorrelationResultCombination::average()
{
	return mAverage;
}

QString ViCorrelationResultCombination::toString()
{
	return "(worst: " + QString::number(mWorst, 'f', 8) + ", best: " + QString::number(mBest, 'f', 8) + ", avg: " + QString::number(mAverage, 'f', 8) + ")";
}

ViCorrelationResult::ViCorrelationResult()
{
	reset();
}

void ViCorrelationResult::reset()
{
	mCrossCorrelation = ViCorrelationResultCombination();
	mSampleCorrelation = ViCorrelationResultCombination();
}

QString ViCorrelationResult::toString()
{
	QString string = "";
	string += "Sample correlation: " + mSampleCorrelation.toString() + "\n";
	string += "Cross correlation: " + mCrossCorrelation.toString();
	return string;
}

void ViCorrelationResult::setCrossCorrelation(ViCorrelationResultCombination combination)
{
	mCrossCorrelation = combination;
}

void ViCorrelationResult::setSampleCorrelation(ViCorrelationResultCombination combination)
{
	mSampleCorrelation = combination;
}

qreal ViCorrelationResult::crossCorrelation(ResultType type)
{
	if(type == ViCorrelationResult::Best)
	{
		return mCrossCorrelation.best();
	}
	if(type == ViCorrelationResult::Worst)
	{
		return mCrossCorrelation.worst();
	}
	if(type == ViCorrelationResult::Average)
	{
		return mCrossCorrelation.average();
	}
}

qreal ViCorrelationResult::sampleCorrelation(ResultType type)
{
	if(type == ViCorrelationResult::Best)
	{
		return mSampleCorrelation.best();
	}
	if(type == ViCorrelationResult::Worst)
	{
		return mSampleCorrelation.worst();
	}
	if(type == ViCorrelationResult::Average)
	{
		return mSampleCorrelation.average();
	}
}
