#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include <QList>
#include <QThreadPool>
#include "vicorrelatorstrategy.h"
#include "viprocessor.h"

class ViCorrelator : public ViDualObserver
{

	public:

		ViCorrelator();
		~ViCorrelator();
		ViCorrelationResult& result();
		void setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData);
		void setWindowSize(int windowSize);
		void initialize();
		void finalize();
		void run();
		
	private:
		
		ViCorrelationResult mResult;
		QList<ViCorrelatorStrategy*> mCorrelators;
		QThreadPool mThreadPool;
		
};

#endif
