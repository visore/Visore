#ifndef VIMATCHERSTRATEGY_H
#define VIMATCHERSTRATEGY_H

#include <QRunnable>
#include "vimatchresult.h"
#include "viaudiobuffer.h"
#include "vipcmconverter.h"

class ViMatcherStrategy : public QRunnable
{

	public:
		
		ViMatcherStrategy();
		virtual void setData(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize);
		virtual void setResult(ViMatchResult *result);
		virtual void initialize(qint32 windowSize = 0);
		virtual void run() = 0;
		virtual void finalize();
		
	protected:

		ViMatchResult *mResult;

		qreal *mFirstData;
		qreal *mSecondData;
		qint32 mFirstSize;
		qint32 mSecondSize;

		qint32 mWindowSize;

};

#endif
