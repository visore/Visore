#include "visingleexecutor.h"

ViSingleExecutor::ViSingleExecutor()
	: ViExecutor()
{
	mNotify = true;
}

void ViSingleExecutor::execute(ViAudioBuffer *buffer, ViProcessor *processor)
{
	mProcessors.clear();
	setBuffer(ViAudioConnection::Input, buffer);
	attach(ViAudioConnection::Input, processor);
	ViSingleExecutor::runIt = &ViSingleExecutor::runObserve;
	ViExecutor::execute();
}

void ViSingleExecutor::execute(ViAudioBuffer *inputBuffer, ViAudioBuffer *outputBuffer, ViProcessor *processor)
{
	mProcessors.clear();
	setBuffer(ViAudioConnection::Input, inputBuffer);
	setBuffer(ViAudioConnection::Output, outputBuffer);
	attach(ViAudioConnection::Input, processor);
	ViSingleExecutor::runIt = &ViSingleExecutor::runModify;
	ViExecutor::execute();
}

void ViSingleExecutor::runNormal()
{
	update();
	do
	{
		mInputChunk->setSize(mReadStream->read(mInputChunk));
		if(mInputChunk->size() > 0)
		{
			(this->*runIt)();
		}
	}
	while(mInputChunk->size() > 0);
}

void ViSingleExecutor::runNotify()
{
	emit progressed(0);
	qint64 size;
	qint64 progress = 0;
	update();
	do
	{
		size = mReadStream->bufferSize();
		mInputChunk->setSize(mReadStream->read(mInputChunk));
		if(mInputChunk->size() > 0)
		{
			(this->*runIt)();
			progress += mInputChunk->size();
			emit progressed((progress * 100) / size);
		}
	}
	while(mInputChunk->size() > 0);
	emit progressed(100);
	emit finished();
}

void ViSingleExecutor::runObserve()
{
	mRealChunk->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mRealChunk->data(), mInputChunk->size()));
	mProcessors.observeInput(mRealChunk);
}

void ViSingleExecutor::runModify()
{
	mRealChunk->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mRealChunk->data(), mInputChunk->size()));
	mProcessors.manipulateInput(mRealChunk);
	mOutputChunk->setSize(mOutputConverter.realToPcm(mRealChunk->data(), mOutputChunk->data(), mRealChunk->size()));
	mWriteStream->write(mOutputChunk);
}
