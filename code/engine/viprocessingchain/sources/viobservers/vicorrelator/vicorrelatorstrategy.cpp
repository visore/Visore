#include "vicorrelatorstrategy.h"

ViCorrelatorStrategy::ViCorrelatorStrategy()
	: ViDualObserver()
{
	setAutoDelete(false); // Object is not automatically deleted by thread pool
	mResult = NULL;
}

void ViCorrelatorStrategy::setResult(ViCorrelationResult *result)
{
	mResult = result;
}
