#include "viprocessorexecutor.h"

ViProcessorExecutor::ViProcessorExecutor()
	: QObject()
{
	QObject::connect(&mThread, SIGNAL(progressed(short)), this, SIGNAL(progressed(short)));
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
	mWindowSize = 0;
}

void ViProcessorExecutor::setWindowSize(int windowSize)
{
	mWindowSize = windowSize;
}

bool ViProcessorExecutor::execute(ViProcessor *processor, ViAudioBuffer *input, ViAudioBuffer *output)
{
	if(dynamic_cast<ViModifier*>(processor) != NULL && output == NULL)
	{
		return false;
	}

	mThread.quit();
	while(mThread.isRunning());
	mThread.reset();
	QObject::disconnect(this, SIGNAL(progressed(short)));
	QObject::disconnect(this, SIGNAL(finished()));

	QObject::connect(this, SIGNAL(progressed(short)), processor, SIGNAL(progressed(short)));
	QObject::connect(this, SIGNAL(finished()), processor, SIGNAL(finished()));
	mThread.attach(ViAudioConnection::Input, processor);

	mThread.setStream(ViAudioConnection::Input, input->createReadStream());
	if(output == NULL)
	{
		mThread.setExecution(ViProcessorThread::Observation);
	}
	else
	{
		mThread.setExecution(ViProcessorThread::Modification);
		mThread.setStream(ViAudioConnection::Output, input->createWriteStream());
	}

	mThread.setNotification(true);
	mThread.setWindowSize(mWindowSize);
	mThread.start();

	return true;
}
