#include "vispectrumanalyzer.h"

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViObserver()
{
}

ViFloatSpectrum& ViSpectrumAnalyzer::spectrum()
{
	return mSpectrum;
}

void ViSpectrumAnalyzer::setWindowFunction(QString functionName)
{
	mWindowFunction = functionName;
}

void ViSpectrumAnalyzer::setWindowSize(int windowSize)
{
	ViObserver::setWindowSize(windowSize);
	mSpectrum.initializeSize((windowSize / 2) + 1);
}

void ViSpectrumAnalyzer::setFormat(ViAudioFormat format)
{
	ViObserver::setFormat(format);
	mSpectrum.initializeFrequencies(format.sampleSize() / 2);
}

void ViSpectrumAnalyzer::run()
{
	/*qint32 index;
	qint32 sampleSize = mBlockSize;
	qint32 halfSize = sampleSize / 2;
	qint32 bytesSize = sampleSize * (mFormat.sampleSize() / 8);
	qint64 totalSize = mSizes.size() * bytesSize;
	
	char bytes[bytesSize];
	double samples[sampleSize];
	double fourier[sampleSize];

	ViFourierTransformer transformer;
	transformer.setWindowFunction(mWindowFunction);
	transformer.setSize(mBlockSize);

	

	emit changed(0);

	while(!mSizes.isEmpty())
	{
		bytesSize = mStream->read(bytes, mSizes.takeFirst());
		sampleSize = pcmToRealPointer(bytes, samples, bytesSize);
		if(sampleSize != mBlockSize)
		{
			for(index = sampleSize; index < mBlockSize; ++index)
			{
				samples[index] = 0;
			}
			sampleSize = mBlockSize;
		}
		transformer.forwardTransform(samples, fourier);
		mSpectrum->add(0, ViComplexFloat(fourier[0], 0));
		for(index = 1; index < halfSize; ++index)
		{
			mSpectrum->add(index, ViComplexFloat(fourier[index], -fourier[index + halfSize]));
		}
		mSpectrum->add(halfSize, ViComplexFloat(fourier[sampleSize - 1], 0));
		emit changed((totalSize - (mSizes.size() * bytesSize)) / (totalSize / 100.0));
	}
	mSpectrum->finalize();*/
}
