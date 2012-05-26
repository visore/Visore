#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"
#include <QThread>

class ViCrossCorrelationMatcherThread : public QThread
{

	public:

		ViCrossCorrelationMatcherThread();
		void setStreams(ViAudioBufferStream *first, ViAudioBufferStream *second);
		void setResult(ViMatchResult *result);
		void run();

	private:

		ViAudioBufferStream *mFirstStream;
		ViAudioBufferStream *mSecondStream;
		ViMatchResult *mResult;

};


class ViCrossCorrelationMatcher : public ViMatcherStrategy
{

	public:
		
		ViCrossCorrelationMatcher();
		void match();

	private:

		ViCrossCorrelationMatcherThread mThread;

};

#endif
