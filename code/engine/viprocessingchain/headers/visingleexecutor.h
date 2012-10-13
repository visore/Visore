#ifndef VISINGLEEXECUTOR_H
#define VISINGLEEXECUTOR_H

#include "viexecutor.h"

class ViSingleExecutor : public ViExecutor
{

	public:

		ViSingleExecutor();
		bool execute(ViAudioObject *object, ViProcessor *processor);

	protected:

		void runNormal();
		void runNotify();
		void runObserve();
		void runModify();
		void runDualObserve();

	private:

		void (ViSingleExecutor::*runIt)();
		ViAudioBufferStream *mReadStream2;

};

#endif
