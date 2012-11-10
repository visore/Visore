#include "vicorrelator.h"

ViCorrelator::ViCorrelator(QString name)
	: ViDualObserver(name)
{
	setAutoDelete(false); // Object is not automatically deleted by thread pool
}

ViCorrelation& ViCorrelator::result()
{
	return mResult;
}
