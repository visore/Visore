#ifndef VISINGLEEXECUTOR_H
#define VISINGLEEXECUTOR_H

#include "viexecutor.h"

class ViSingleExecutor : public ViExecutor
{

	public:

		ViSingleExecutor();
		void execute(ViAudioBuffer *buffer, ViProcessor *processor);
		void execute(ViAudioBuffer *inputBuffer, ViAudioBuffer *outputBuffer, ViProcessor *processor);

	protected:

		void runNormal();
		void runNotify();
		void runObserve();
		void runModify();

	private:

		void (ViSingleExecutor::*runIt)();

};

#endif
