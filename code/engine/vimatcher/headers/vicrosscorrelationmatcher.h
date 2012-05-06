#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"

class ViCrossCorrelationMatcher : public ViMatcherStrategy
{

	public:
		
		ViCrossCorrelationMatcher();
		void match();
		
};

#endif
