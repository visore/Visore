#include "vispectrumanalyzer.h"
#include "viaudioposition.h"

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViObserver()
{
	mMilliseconds = 100;
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
}

void ViSpectrumAnalyzer::finalize()
{
	if(mBuffer != NULL)
	{
		delete [] mBuffer;
		mBuffer = NULL;
	}
}

void ViSpectrumAnalyzer::run()
{
	int size = mData->size();
	qreal *data = mData->data();
	int bufferSize;
	do
	{
		bufferSize = mWindowSize - mBufferSize;
		if(bufferSize > 0)
		{
			memcpy(mBuffer, data, sizeof(qreal) * bufferSize);
			data += bufferSize;
			mBufferSize += bufferSize;
		}
		else
		{
			break; //Need more data to do a FFT
		}
		
		if(mBufferSize == mWindowSize)
		{
			double fourier[mWindowSize];
			mTransformer.forwardTransform(mBuffer, fourier);
			ViRealSpectrum spectrum;
			spectrum.initialize(mHalfWindowSize + 1, mFormat.sampleRate() / 2);
			spectrum.add(0, ViRealComplex(fourier[0], 0));
			for(int i = 1; i < mHalfWindowSize; ++i)
			{
				spectrum.add(i, ViRealComplex(fourier[i], -fourier[i + mHalfWindowSize]));
			}
			spectrum.add(mHalfWindowSize, ViRealComplex(fourier[mHalfWindowSize], 0));
			spectrum.finalize();
			mCurrentPosition += mMilliseconds;
			emit changed(spectrum, mCurrentPosition);
			size -= mBufferSize;
			mBufferSize = 0;
		}
	}
	while(size > 0);


/*
	int index;
	bool tooSmall = false;
	double *samples;
	if(mData->size() != mWindowSize)
	{
		double *paddedSamples = new double[mWindowSize];
		memcpy(paddedSamples, mData->data(), sizeof(double) * mData->size());
		for(index = mData->size(); index < mWindowSize; ++index)
		{
			paddedSamples[index] = 0;
		}
		tooSmall = true;
		samples = paddedSamples;
	}
	else
	{
		samples = mData->data();
	}

	double fourier[mWindowSize];
	mTransformer.forwardTransform(samples, fourier);

	mMutex.lock();
	mSpectrum.add(0, ViRealComplex(fourier[0], 0));
	for(index = 1; index < mHalfWindowSize; ++index)
	{
		mSpectrum.add(index, ViRealComplex(fourier[index], -fourier[index + mHalfWindowSize]));
	}
	mSpectrum.add(mHalfWindowSize, ViRealComplex(fourier[mHalfWindowSize], 0));
	mMutex.unlock();

	if(tooSmall)
	{
		delete [] samples;
	}*/
}
