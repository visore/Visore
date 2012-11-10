#include "vicorrelator.h"

ViCorrelator::ViCorrelator()
	: ViDualObserver()
{
	setAutoDelete(false); // Object is not automatically deleted by thread pool
}

ViCorrelation& ViCorrelator::result()
{
	return mResult;
}
