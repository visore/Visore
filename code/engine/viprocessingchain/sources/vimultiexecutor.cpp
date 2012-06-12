#include "vimultiexecutor.h"

void ViMultiExecutor::runNormal()
{
	initialize();
	do
	{
		mInputChunk->setSize(mReadStream->read(mInputChunk));
		if(mInputChunk->size() > 0)
		{
			mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), mInputChunk->size()));
			ViSampleChunk::copy(mInputSamples, mOutputSamples);
			mProcessors.observeInput(mOutputSamples);
			mProcessors.manipulateInput(mOutputSamples);
			mProcessors.observeOutput(mOutputSamples);
			mProcessors.observeDual(mInputSamples, mOutputSamples);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mOutputSamples->data(), mOutputChunk->data(), mOutputSamples->size()));
			mWriteStream->write(mOutputChunk);
		}
	}
	while(mInputChunk->size() > 0);
	finalize();
}

void ViMultiExecutor::runNotify()
{
	emit progressed(0);
	qint64 size;
	qint64 progress = 0;
	initialize();
	do
	{
		size = mReadStream->bufferSize();
		mInputChunk->setSize(mReadStream->read(mInputChunk));
		if(mInputChunk->size() > 0)
		{
			mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), mInputChunk->size()));
			ViSampleChunk::copy(mInputSamples, mOutputSamples);
			mProcessors.observeInput(mOutputSamples);
			mProcessors.manipulateInput(mOutputSamples);
			mProcessors.observeOutput(mOutputSamples);
			mProcessors.observeDual(mInputSamples, mOutputSamples);
			mOutputChunk->setSize(mOutputConverter.realToPcm(mOutputSamples->data(), mOutputChunk->data(), mOutputSamples->size()));
			mWriteStream->write(mOutputChunk);
			progress += mInputChunk->size();
			emit progressed((progress * 100) / size);
		}
	}
	while(mInputChunk->size() > 0);
	finalize();
	emit progressed(100);
}
