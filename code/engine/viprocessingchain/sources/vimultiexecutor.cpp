#include "vimultiexecutor.h"

void ViMultiExecutor::runNormal()
{
	int size;
	do
	{
		size = mReadStream->read(mInputChunk->data(), mInputChunk->size());
		if(size > 0)
		{
			mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), size));
			ViSampleChunk::copy(mInputSamples, mOutputSamples);
			mProcessors.observeInput(mOutputSamples);
			mProcessors.manipulateInput(mOutputSamples);
			mProcessors.observeOutput(mOutputSamples);
			mProcessors.observeDual(mInputSamples, mOutputSamples);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mOutputSamples->data(), mOutputChunk->data(), mOutputSamples->size()));
			mWriteStream->write(mOutputChunk);
		}
	}
	while(size > 0);
}

void ViMultiExecutor::runNotify()
{
	emit progressed(0);
	qint64 bufferSize;
	qint64 progress = 0;
	int size;
	do
	{
		bufferSize = mReadStream->bufferSize();
		size = mReadStream->read(mInputChunk->data(), mInputChunk->size());
		if(mInputChunk->size() > 0)
		{
			mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), size));
			ViSampleChunk::copy(mInputSamples, mOutputSamples);
			mProcessors.observeInput(mOutputSamples);
			mProcessors.manipulateInput(mOutputSamples);
			mProcessors.observeOutput(mOutputSamples);
			mProcessors.observeDual(mInputSamples, mOutputSamples);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mOutputSamples->data(), mOutputChunk->data(), mOutputSamples->size()));
			mWriteStream->write(mOutputChunk);
			progress += mInputChunk->size();
			emit progressed((progress * 100) / bufferSize);
		}
	}
	while(size > 0);
	emit progressed(100);
}
