#include "vispectrumanalyzer.h"

ViSpectrumAnalyzerThread::ViSpectrumAnalyzerThread()
{
	mPreviousStop = 0;
	mWindow == NULL;
}

ViSpectrumAnalyzerThread::~ViSpectrumAnalyzerThread()
{
	for(int i = 0; i < mSamples.size(); ++i)
	{
		delete [] mSamples[i];
	}
	mSamples.clear();
	mChunks.clear();
	delete mWindow;
}

void ViSpectrumAnalyzerThread::run()
{
	mMutex.lock();
	while(!mSamples.isEmpty())
	{
		float *input = mSamples.takeFirst();
		for(int i = 0; i < SPECTRUM_SAMPLES; ++i)
		{
			input[i] *= mWindow->at(i);
		}

		float output[SPECTRUM_SAMPLES];
		mFourierWrapper.calculateFft(output, input);

		delete [] input;
		//addWindow();
	}
	mMutex.unlock();
}

void ViSpectrumAnalyzerThread::setWindowFunction(ViWindowFunction *windowFunction)
{
	mWindowFunction = windowFunction;
	calculateWindow(SPECTRUM_SAMPLES);
}

void ViSpectrumAnalyzerThread::addChunk(QSharedPointer<ViWaveFormChunk> chunk)
{
	mChunks.append(chunk);
	while(enoughSamplesAvailable())
	{
		float *data = new float[SPECTRUM_SAMPLES];
		qint16 currentSize = 0;
		while(currentSize != SPECTRUM_SAMPLES)
		{
			QSharedPointer<ViWaveFormChunk> first = mChunks.first();
			qreal *firstData = first->data();
			qint64 difference = first->size() - mPreviousStop;

			if(difference == SPECTRUM_SAMPLES)
			{
				qint16 counter = currentSize;
				for(qint64 i = mPreviousStop; i < difference; ++i)
				{
					data[counter] = firstData[i];
					++counter;
				}
				mPreviousStop = 0;
				mChunks.removeFirst();
				currentSize += difference;
			}
			else if(difference > SPECTRUM_SAMPLES)
			{
				qint16 counter = currentSize;
				for(qint64 i = mPreviousStop; i < SPECTRUM_SAMPLES; ++i)
				{
					data[counter] = firstData[i];
					++counter;
				}
				mPreviousStop += SPECTRUM_SAMPLES;
				currentSize += SPECTRUM_SAMPLES;
			}
			else if(difference < SPECTRUM_SAMPLES)
			{
				qint16 counter = currentSize;
				for(qint64 i = mPreviousStop; i < difference; ++i)
				{
					data[counter] = firstData[i];
					++counter;
				}
				mPreviousStop = 0;
				mChunks.removeFirst();
				currentSize += difference;
			}
		}
		mMutex.lock();
		mSamples.append(data);
		mMutex.unlock();
	}

	if(!isRunning())
	{
		start();
	}
}

void ViSpectrumAnalyzerThread::calculateWindow(qint64 size)
{
	mWindow = new ViSpectrumWindow(size, mWindowFunction);
}

bool ViSpectrumAnalyzerThread::enoughSamplesAvailable()
{
	qint64 total = 0;
	for(int i = 0; i < mChunks.size(); ++i)
	{
		total += mChunks[i]->size();
		if(total >= SPECTRUM_SAMPLES)
		{
			return true;
		}
	}
	return false;
}

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
{
	mThread = new ViSpectrumAnalyzerThread();
}

ViSpectrumAnalyzer::~ViSpectrumAnalyzer()
{
	if(mThread != NULL)
	{
		mThread->quit();
		delete mThread;
		mThread = NULL;
	}
}

void ViSpectrumAnalyzer::initialize(ViAudioBuffer *buffer)
{
}

void ViSpectrumAnalyzer::setWindowFunction(ViWindowFunction *windowFunction)
{
	mThread->setWindowFunction(windowFunction);
}

void ViSpectrumAnalyzer::start(QSharedPointer<ViWaveFormChunk> chunk)
{
	mThread->addChunk(chunk);
}

void ViSpectrumAnalyzer::stop()
{

}

bool ViSpectrumAnalyzer::isReady()
{

}
