#ifndef VICORRELATORSTRATEGY_H
#define VICORRELATORSTRATEGY_H

#include <QRunnable>
#include "vicorrelationresult.h"
#include "viaudiobuffer.h"
#include "vipcmconverter.h"

class ViCorrelatorStrategy : public QRunnable
{

	public:
		
		ViCorrelatorStrategy();
		virtual void setData(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize);
		virtual void setResult(ViCorrelationResult *result);
		virtual void initialize(qint32 windowSize = 0);
		virtual void run() = 0;
		virtual void finalize();
		
	protected:

		ViCorrelationResult *mResult;

		qreal *mFirstData;
		qreal *mSecondData;
		qint32 mFirstSize;
		qint32 mSecondSize;

		qint32 mWindowSize;

};

#endif
