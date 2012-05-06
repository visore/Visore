#ifndef VIMATCHER_H
#define VIMATCHER_H

#include <QList>
#include <QThread>
#include "vimatcherstrategy.h"

class ViMatcherThread : public QThread
{

	public:

		ViMatcherThread(ViMatcherStrategy *strategy);
		void run();

	private:

		ViMatcherStrategy *mStrategy;

};

class ViMatcher : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	private slots:

		void checkFinished();

	public:

		ViMatcher();
		~ViMatcher();

		void match(ViAudioBuffer *first, ViAudioBuffer *second);
		ViMatchResult result();
		
	private:
		
		ViMatchResult mResult;
		QList<ViMatcherStrategy*> mStrategies;
		QList<ViMatcherThread*> mThreads;
		qint32 finishCounter;
		
};

#endif
