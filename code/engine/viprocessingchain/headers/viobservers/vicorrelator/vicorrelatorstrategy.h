#ifndef VICORRELATORSTRATEGY_H
#define VICORRELATORSTRATEGY_H

#include "vicorrelationresult.h"
#include "viaudiobuffer.h"
#include "vipcmconverter.h"
#include "viprocessor.h"

class ViCorrelatorStrategy : public ViDualObserver
{

	public:
		
		ViCorrelatorStrategy();
		virtual void setResult(ViCorrelationResult *result);
		
	protected:

		ViCorrelationResult *mResult;

};

#endif
