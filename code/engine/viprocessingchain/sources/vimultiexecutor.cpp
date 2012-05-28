#include "vimultiexecutor.h"

void ViMultiExecutor::runNormal()
{
	update();
	do
	{
		mInputChunk->setSize(mReadStream->read(mInputChunk));
		if(mInputChunk->size() > 0)
		{
			mRealChunk->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mRealChunk->data(), mInputChunk->size()));
			mProcessors.observeInput(mRealChunk);
			mProcessors.manipulateInput(mRealChunk);
			mProcessors.observeOutput(mRealChunk);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mRealChunk->data(), mOutputChunk->data(), mRealChunk->size()));
			mWriteStream->write(mOutputChunk);
		}
	}
	while(mInputChunk->size() > 0);
}

void ViMultiExecutor::runNotify()
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
			mRealChunk->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mRealChunk->data(), mInputChunk->size()));
			mProcessors.observeInput(mRealChunk);
			mProcessors.manipulateInput(mRealChunk);
			mProcessors.observeOutput(mRealChunk);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mRealChunk->data(), mOutputChunk->data(), mRealChunk->size()));
			mWriteStream->write(mOutputChunk);
			progress += mInputChunk->size();
			emit progressed((progress * 100) / size);
		}
	}
	while(mInputChunk->size() > 0);
	emit progressed(100);
	emit finished();
}
