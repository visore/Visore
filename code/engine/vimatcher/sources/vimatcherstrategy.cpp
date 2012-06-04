#include "vimatcherstrategy.h"

ViMatcherStrategy::ViMatcherStrategy()
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

void ViMatcherStrategy::setData(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize)
{
	mFirstData =  firstData;
	mSecondData = secondData;
	mFirstSize = firstSize;
	mSecondSize = secondSize;
}

void ViMatcherStrategy::setResult(ViMatchResult *result)
{
	mResult = result;
}

void ViMatcherStrategy::initialize(qint32 windowSize)
{
	mWindowSize = windowSize;
}

void ViMatcherStrategy::finalize()
{
}
