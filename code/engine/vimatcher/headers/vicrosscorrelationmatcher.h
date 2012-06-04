#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"
#include <QThread>

class ViCrossCorrelationMatcher : public ViMatcherStrategy
{

	public:
		
		ViCrossCorrelationMatcher();
		void match();

	protected:

		qreal norm(qreal array[], qint32 size);
		void applyNorm(qreal array[], qint32 size, qreal norm);

};

#endif
