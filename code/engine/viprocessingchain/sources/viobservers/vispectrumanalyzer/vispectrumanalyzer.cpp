#include "vispectrumanalyzer.h"

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViObserver()
{
	mHalfWindowSize = 0;
}

ViRealSpectrum ViSpectrumAnalyzer::spectrum()
{
	mMutex.lock();
	mSpectrum.finalize();
	ViRealSpectrum result = mSpectrum;
	mMutex.unlock();
	return result;
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
	mSpectrum.initialize(mHalfWindowSize + 1, mFormat.sampleRate() / 2);
}

void ViSpectrumAnalyzer::setFormat(ViAudioFormat format)
{
	ViObserver::setFormat(format);
	mSpectrum.initialize(mHalfWindowSize + 1, mFormat.sampleRate() / 2);
}

void ViSpectrumAnalyzer::run()
{
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
	}
}
