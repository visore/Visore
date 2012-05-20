#include "vispectrumanalyzer.h"
#include "vihammingwindower.h"

#define FFT_CHUNK_SIZE 4096

ViSpectrumAnalyzerThread::ViSpectrumAnalyzerThread()
	: QThread()
{
}

void ViSpectrumAnalyzerThread::setData(ViAudioBuffer *buffer, ViFrequencySpectrum *spectrum)
{
	mFormat = buffer->format();
	qint32 sampleSize = mFormat.sampleSize();
	if(sampleSize == 8)
	{
		pcmToRealPointer = &ViPcmConverter<float>::pcmToReal8;
	}
	else if(sampleSize == 16)
	{
		pcmToRealPointer = &ViPcmConverter<float>::pcmToReal16;
	}
	else if(sampleSize == 24)
	{
		pcmToRealPointer = &ViPcmConverter<float>::pcmToReal24;
	}
	else if(sampleSize == 32)
	{
		pcmToRealPointer = &ViPcmConverter<float>::pcmToReal32;
	}

	mStream = buffer->createReadStream();
	mSpectrum = spectrum;
}

void ViSpectrumAnalyzerThread::setPositions(qint64 start, qint64 end)
{
	mStart = start;
	qint32 optimal = FFT_CHUNK_SIZE * (mFormat.sampleSize() / 8);
	while((end - start) > optimal)
	{
		mSizes.append(optimal);
		end -= optimal;
	}
	if(end > 0)
	{
		mSizes.append(end);
	}
}

void ViSpectrumAnalyzerThread::run()
{
	qint32 index;
	qint32 halfSize;
	qint32 sampleSize = FFT_CHUNK_SIZE;
	qint32 bytesSize = sampleSize * (mFormat.sampleSize() / 8);
	
	char bytes[bytesSize];
	float samples[sampleSize];
	float fourier[sampleSize];

	ViFourierTransformer transformer;
	transformer.setFixedSize(FFT_CHUNK_SIZE);
	ViWindower *windower = NULL;//ViHammingWindower::instance();
	
	while(!mSizes.isEmpty())
	{
		bytesSize = mStream->read(bytes, mSizes.takeFirst());
		sampleSize = pcmToRealPointer(bytes, samples, bytesSize);
		if(sampleSize != FFT_CHUNK_SIZE)
		{
			for(index = sampleSize; index < FFT_CHUNK_SIZE; ++index)
			{
				samples[index] = 0;
			}
			sampleSize = FFT_CHUNK_SIZE;
		}
		transformer.forwardTransform(samples, fourier, 4096, windower);
		halfSize = sampleSize / 2;
		mSpectrum->lock();
		mSpectrum->append(ViComplexFloat(fourier[0], 0));
		for(index = 1; index < halfSize; ++index)
		{
			mSpectrum->append(ViComplexFloat(fourier[index], -fourier[index + halfSize]));
		}
		mSpectrum->append(ViComplexFloat(fourier[sampleSize - 1], 0));
		mSpectrum->unlock();
	}

}

ViSpectrumAnalyzer::ViSpectrumAnalyzer(ViAudioBuffer *buffer)
	: QObject()
{
	mBuffer = buffer;
	mThread.setData(buffer, &mSpectrum);
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

void ViSpectrumAnalyzer::analyze()
{
	mThread.setPositions(0, mBuffer->size());
	mThread.start();
}

void ViSpectrumAnalyzer::analyze(ViAudioPosition start, ViAudioPosition end)
{
	mThread.setPositions(start.position(ViAudioPosition::Bytes), end.position(ViAudioPosition::Bytes));
	mThread.start();
}

ViFrequencySpectrum& ViSpectrumAnalyzer::spectrum()
{
	return mSpectrum;
}
