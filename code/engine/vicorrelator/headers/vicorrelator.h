#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include <QList>
#include <QThread>
#include <QThreadPool>
#include "vicorrelatorstrategy.h"

class ViCorrelatorThread : public QThread
{
	Q_OBJECT

	signals:

		void changed(qreal percentage);

	public:

		ViCorrelatorThread();
		~ViCorrelatorThread();
		void setBuffers(ViAudioBuffer *first, ViAudioBuffer *second);
		void setResult(ViCorrelationResult *result);
		void setWindowSize(qint32 windowSize);
		void run();

	protected:

		void initialize();
		void correlate(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize);
		void finalize();

	private:

		QThreadPool *mGlobalPool;
		ViAudioBufferStream *mFirstStream;
		ViAudioBufferStream *mSecondStream;
		ViCorrelationResult *mResult;
		QList<ViCorrelatorStrategy*> mCorrelators;
		qint32 mWindowSize;

};

class ViCorrelator : public QObject
{
	Q_OBJECT

	signals:

		void finished();
		void changed(qreal percentage);

	public:

		ViCorrelator();
		void correlate(ViAudioBuffer *first, ViAudioBuffer *second, qint32 windowSize = 4096);
		ViCorrelationResult& result();
		
	private:
		
		ViCorrelationResult mResult;
		ViCorrelatorThread mThread;
		
};

#endif
