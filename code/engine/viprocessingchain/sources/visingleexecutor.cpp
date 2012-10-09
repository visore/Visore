#include "visingleexecutor.h"

ViSingleExecutor::ViSingleExecutor()
	: ViExecutor()
{
	mNotify = true;
}

bool ViSingleExecutor::execute(ViAudioBuffer *buffer, ViProcessor *processor)
{
	mProcessors.clear();
	if(buffer == NULL)
	{
		return false;
	}
	if(dynamic_cast<ViDualObserver*>(processor) != NULL)
	{
		return false;
	}
	else if(dynamic_cast<ViObserver*>(processor) != NULL)
	{
		attach(ViAudio::AudioInput, processor);
		ViSingleExecutor::runIt = &ViSingleExecutor::runObserve;
	}
	else if(dynamic_cast<ViDualObserver*>(processor) != NULL)
	{
		attach(ViAudio::AudioOutput, processor);
		setBuffer(ViAudio::AudioOutput, buffer);
		ViSingleExecutor::runIt = &ViSingleExecutor::runModify;
	}
	setBuffer(ViAudio::AudioInput, buffer);
	ViExecutor::execute();
	return true;
}

bool ViSingleExecutor::execute(ViAudioBuffer *inputBuffer, ViAudioBuffer *outputBuffer, ViProcessor *processor)
{
	mProcessors.clear();
	if(dynamic_cast<ViDualObserver*>(processor) != NULL)
	{
		if(inputBuffer == NULL || outputBuffer == NULL)
		{
			return false;
		}
		mReadStream2 = outputBuffer->createReadStream();
		attach(ViAudio::AudioInputOutput, processor);
		ViSingleExecutor::runIt = &ViSingleExecutor::runDualObserve;
	}
	else if(dynamic_cast<ViObserver*>(processor) != NULL)
	{
		if(inputBuffer == NULL)
		{
			return false;
		}
		attach(ViAudio::AudioInput, processor);
		ViSingleExecutor::runIt = &ViSingleExecutor::runObserve;
	}
	else if(dynamic_cast<ViModifier*>(processor) != NULL)
	{
		if(inputBuffer == NULL || outputBuffer == NULL)
		{
			return false;
		}
		attach(ViAudio::AudioOutput, processor);
		ViSingleExecutor::runIt = &ViSingleExecutor::runModify;
	}
	setBuffer(ViAudio::AudioInput, inputBuffer);
	setBuffer(ViAudio::AudioOutput, outputBuffer);
	ViExecutor::execute();
	return true;
}

void ViSingleExecutor::runNormal()
{
	do
	{
		mReadStream->read(*mInputChunk);
		if(mInputChunk->size() > 0)
		{
			(this->*runIt)();
		}
	}
	while(mInputChunk->size() > 0);
	finalize();
}

void ViSingleExecutor::runNotify()
{
	emit progressed(0);
	qint64 size;
	qint64 progress = 0;
	do
	{
		size = mReadStream->size();
		mReadStream->read(*mInputChunk);
		if(mInputChunk->size() > 0)
		{
			(this->*runIt)();
			progress += mInputChunk->size();
			emit progressed((progress * 100) / size);
		}
	}
	while(mInputChunk->size() > 0);
	finalize();
	emit progressed(100);
}

void ViSingleExecutor::runObserve()
{
	mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), mInputChunk->size()));
	mProcessors.observeInput(mInputSamples);
	mProcessedSamples += mInputSamples->size();
}

void ViSingleExecutor::runModify()
{
	mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), mInputChunk->size()));
	mProcessors.manipulateInput(mInputSamples);
	mOutputChunk->setSize(mOutputConverter.realToPcm(mInputSamples->data(), mOutputChunk->data(), mInputSamples->size()));
	mWriteStream->write(*mOutputChunk);
	mProcessedSamples += mInputSamples->size();
}

void ViSingleExecutor::runDualObserve()
{
	mInputSamples->setSize(mInputConverter.pcmToReal(mInputChunk->data(), mInputSamples->data(), mInputChunk->size()));
	mReadStream2->read(*mOutputChunk);
	mOutputSamples->setSize(mInputConverter.pcmToReal(mOutputChunk->data(), mOutputSamples->data(), mOutputChunk->size()));
	mProcessors.observeDual(mInputSamples, mOutputSamples);
	mProcessedSamples += mInputSamples->size();
}
