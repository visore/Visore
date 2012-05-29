#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include <QList>
#include <QThreadPool>
#include "vicorrelatorstrategy.h"
#include "viprocessor.h"

class ViCorrelator : public ViObserver
{

	public:

		ViCorrelator();
		~ViCorrelator();
		ViCorrelationResult& result();
		void setWindowSize(int windowSize);
		void run();
		
	private:
		
		ViCorrelationResult mResult;
		QList<ViCorrelatorStrategy*> mCorrelators;
		QThreadPool mThreadPool;
		
};

#endif
