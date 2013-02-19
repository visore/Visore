#include <vispectrumanalyzer.h>
#include <viaudioposition.h>

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViProcessor()
{
	mMilliseconds = 150;
	mInterval = 0;
	mWindowSize = 0;
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

void ViSpectrumAnalyzer::setInterval(int milliseconds)
{
	mMilliseconds = milliseconds;
}

void ViSpectrumAnalyzer::initialize()
{
	mWindowSize = samples().size();
	mTransformer.setSize(mWindowSize);
	mHalfWindowSize = mWindowSize / 2;
	mInterval = ViAudioPosition::convertPosition(mMilliseconds, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, format1());
	if(mBuffer != NULL)
	{
		delete [] mBuffer;
	}
	mBuffer = new qreal[mWindowSize];
	mBufferSize = 0;
	mCurrentPosition = 0;
	mSampleCounter = 0;
	mSpectrum.setInterval(ViAudioPosition(mMilliseconds, ViAudioPosition::Milliseconds, format1()));
	mSpectrum.initialize(mHalfWindowSize + 1, format1().sampleRate() / 2);
}

void ViSpectrumAnalyzer::execute()
{
	ViSampleChunk &theSamples = samples();

	int size = theSamples.size();
	qreal *data = theSamples.data();
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
				mSpectrum.initialize(mHalfWindowSize + 1, format1().sampleRate() / 2);
			}
			size -= mBufferSize;
			mBufferSize = 0;
		}
	}
	while(size > 0);
}

void ViSpectrumAnalyzer::finalize()
{
	if(mBuffer != NULL)
	{
		delete [] mBuffer;
		mBuffer = NULL;
	}
}
