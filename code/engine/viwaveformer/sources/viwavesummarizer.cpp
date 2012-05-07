#include "viwavesummarizer.h"

#define MAX_SIGNAL_COUNTER 100

ViWaveSummarizerThread::ViWaveSummarizerThread(ViAudioBuffer *buffer, ViWaveForm *form)
	: ViProcessorThread(buffer)
{
	mForm = form;
	mFormer.setSampleSize(mBuffer->format().sampleSize());
}

void ViWaveSummarizerThread::run()
{
	mSizesMutex.lock();
	bool isEmpty = mSizes.isEmpty();
	mSizesMutex.unlock();
	mSignalCounter = 0;

	while(!isEmpty)
	{
		mSizesMutex.lock();
		int size = mSizes.takeFirst();
		mSizesMutex.unlock();

		ViAudioBufferChunk chunk;
		mReadStream->read(&chunk, size);
		ViWaveFormChunk wave;
		mFormer.pcmToReal(&chunk, &wave);

		float *data = wave.data();
		size = wave.size();
		for(int i = 0; i < size; ++i)
		{
			mForm->append(data[i]);
		}

		++mSignalCounter;
		if(mSignalCounter > MAX_SIGNAL_COUNTER)
		{
			mSignalCounter = 0;
			emit changed();
		}

		mSizesMutex.lock();
		isEmpty = mSizes.isEmpty();
		mSizesMutex.unlock();
	}
	emit changed();
}

ViWaveSummarizer::ViWaveSummarizer()
	: ViProcessor()
{
}

void ViWaveSummarizer::initialize(ViAudioBuffer *buffer)
{
	mThread = new ViWaveSummarizerThread(buffer, &mForm);
	ViProcessor::initialize(buffer);
}

ViWaveForm* ViWaveSummarizer::waveSummary()
{
	return &mForm;
}
