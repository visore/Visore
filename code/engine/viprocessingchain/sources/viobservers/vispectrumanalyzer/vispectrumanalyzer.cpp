#include "vispectrumanalyzer.h"
#include "viaudioposition.h"
#include <string>

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViObserver()
{
	mMilliseconds = 150;
	mInterval = 0;
	mHalfWindowSize = 0;
	mBuffer = NULL;
}

ViSpectrumAnalyzer::~ViSpectrumAnalyzer()
{
	if(mBuffer != NULL)
	{
		delete [] mBuffer;
		mBuffer = NULL;
	}
}

void ViSpectrumAnalyzer::setWindowFunction(QString functionName)
{
	mTransformer.setWindowFunction(functionName);
}

void ViSpectrumAnalyzer::setWindowSize(int windowSize)
{
	ViObserver::setWindowSize(windowSize);
	mTransformer.setSize(mWindowSize);
	mHalfWindowSize = mWindowSize / 2;
}

void ViSpectrumAnalyzer::setInterval(int milliseconds)
{
	mMilliseconds = milliseconds;
}

void ViSpectrumAnalyzer::initialize()
{
	mInterval = ViAudioPosition::convertPosition(mMilliseconds, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mBuffer = new qreal[mWindowSize];
	mBufferSize = 0;
	mCurrentPosition = 0;
	mSampleCounter = 0;
	mSpectrum.setInterval(ViAudioPosition(mMilliseconds, ViAudioPosition::Milliseconds, mFormat));
	mSpectrum.initialize(mHalfWindowSize + 1, mFormat.sampleRate() / 2);
}

void ViSpectrumAnalyzer::finalize()
{
	if(mBuffer != NULL)
	{
		delete [] mBuffer;
		mBuffer = NULL;
	}
}

void ViSpectrumAnalyzer::execute()
{
	int size = mData->size();
	qreal *data = mData->data();
	int bufferSize;
	do
	{
		bufferSize = mWindowSize - mBufferSize;
		if(bufferSize > 0 && size >= bufferSize)
		{
			memcpy(mBuffer, data, sizeof(qreal) * bufferSize);
			mBufferSize += bufferSize;
			mSampleCounter += bufferSize;
		}
		else if(bufferSize > 0 && size < bufferSize)
		{
			memcpy(mBuffer, data, sizeof(qreal) * size);
			mBufferSize += size;
			mSampleCounter += size;
			break; //Need more data to do a FFT
		}
		else
		{
			break; //Need more data to do a FFT
		}
		
		if(mBufferSize == mWindowSize)
		{
			double fourier[mWindowSize];
			mTransformer.forwardTransform(mBuffer, fourier);
			
			mSpectrum.add(0, ViRealComplex(fourier[0], 0));
			for(int i = 1; i < mHalfWindowSize; ++i)
			{
				mSpectrum.add(i, ViRealComplex(fourier[i], -fourier[i + mHalfWindowSize]));
			}
			mSpectrum.add(mHalfWindowSize, ViRealComplex(fourier[mHalfWindowSize], 0));

			if(mSampleCounter >= mInterval)
			{
				mCurrentPosition += mMilliseconds;
				mSampleCounter = 0;
				mSpectrum.finalize();
				emit changed(mSpectrum, mCurrentPosition);
				mSpectrum.clear();
				mSpectrum.initialize(mHalfWindowSize + 1, mFormat.sampleRate() / 2);
			}
			size -= mBufferSize;
			mBufferSize = 0;
		}
	}
	while(size > 0);
}
