#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include "vicorrelation.h"
#include "vibuffer.h"
#include "vipcmconverter.h"
#include "viprocessor.h"

class ViCorrelator : public ViDualProcessor
{

	public:
		
		ViCorrelator();
		ViCorrelation& result();
		
	protected:

		ViCorrelation mResult;

};

#endif
