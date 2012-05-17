#ifndef VIMATCHERSTRATEGY_H
#define VIMATCHERSTRATEGY_H

#include "vimatchresult.h"
#include "viaudiobuffer.h"
#include "vipcmconverter.h"

class ViMatcherStrategy : public QObject
{
	Q_OBJECT

	public:
		
		ViMatcherStrategy();
		virtual void setBuffers(ViAudioBuffer *first, ViAudioBuffer *second);
		virtual void setResult(ViMatchResult *result);
		virtual void match() = 0;
		
	protected:

		ViAudioBufferStream *mFirstStream;
		ViAudioBufferStream *mSecondStream;
		ViMatchResult *mResult;

};

#endif
