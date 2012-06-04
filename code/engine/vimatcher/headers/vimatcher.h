#ifndef VIMATCHER_H
#define VIMATCHER_H

#include <QList>
#include <QThread>
#include <QThreadPool>
#include "vimatcherstrategy.h"

class ViMatcherThread : public QThread
{
	Q_OBJECT

	signals:

		void changed(qreal percentage);

	public:

		ViMatcherThread();
		~ViMatcherThread();
		void setBuffers(ViAudioBuffer *first, ViAudioBuffer *second);
		void setResult(ViMatchResult *result);
		void setWindowSize(qint32 windowSize);
		void run();

	protected:

		void initialize();
		void match(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize);
		void finalize();

	private:

		QThreadPool *mGlobalPool;
		ViAudioBufferStream *mFirstStream;
		ViAudioBufferStream *mSecondStream;
		ViMatchResult *mResult;
		QList<ViMatcherStrategy*> mMatchers;
		qint32 mWindowSize;

};

class ViMatcher : public QObject
{
	Q_OBJECT

	signals:

		void finished();
		void changed(qreal percentage);

	public:

		ViMatcher();
		void match(ViAudioBuffer *first, ViAudioBuffer *second, qint32 windowSize = 4096);
		ViMatchResult result();
		
	private:
		
		ViMatchResult mResult;
		ViMatcherThread mThread;
		
};

#endif
