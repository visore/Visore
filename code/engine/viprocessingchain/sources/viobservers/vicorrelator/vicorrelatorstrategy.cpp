#include "vicorrelatorstrategy.h"

ViCorrelatorStrategy::ViCorrelatorStrategy()
	: QRunnable()
{
	setAutoDelete(false); // Object is not automatically deleted by thread pool

	mResult = NULL;
	mFirstData = NULL;
	mSecondData = NULL;
	mFirstSize = 0;
	mSecondSize = 0;
	mWindowSize = 0;
}

void ViCorrelatorStrategy::setData(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize)
{
	mFirstData =  firstData;
	mSecondData = secondData;
	mFirstSize = firstSize;
	mSecondSize = secondSize;
}

void ViCorrelatorStrategy::setResult(ViCorrelationResult *result)
{
	mResult = result;
}

void ViCorrelatorStrategy::initialize(qint32 windowSize)
{
	mWindowSize = windowSize;
}

void ViCorrelatorStrategy::finalize()
{
}
