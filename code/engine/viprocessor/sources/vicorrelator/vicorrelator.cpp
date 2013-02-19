#include "vicorrelator.h"

ViCorrelator::ViCorrelator()
	: ViDualProcessor()
{
}

ViCorrelation& ViCorrelator::result()
{
	return mResult;
}
