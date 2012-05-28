#ifndef VIPROCESSOREXECUTOR_H
#define VIPROCESSOREXECUTOR_H

#include "viaudiobuffer.h"
#include "viprocessorthread.h"

class ViProcessorExecutor : public QObject
{
    Q_OBJECT

	signals:

		void progressed(short progress);
		void finished();

	public:

		ViProcessorExecutor();
		void setWindowSize(int windowSize);
		bool execute(ViProcessor *processor, ViAudioBuffer *input, ViAudioBuffer *output = NULL);

	private:

		ViProcessorThread mThread;
		int mWindowSize;

};

#endif
