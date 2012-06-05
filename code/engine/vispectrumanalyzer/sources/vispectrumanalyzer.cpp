#include "vispectrumanalyzer.h"

#define DEFAULT_BLOCK_SIZE 4096

ViSpectrumAnalyzerThread::ViSpectrumAnalyzerThread()
	: QThread()
{
	mWindowFunction = "";
}

void ViSpectrumAnalyzerThread::setData(ViAudioBuffer *buffer, ViFloatSpectrum *spectrum)
{
	mFormat = buffer->format();
	qint32 sampleSize = mFormat.sampleSize();
	if(sampleSize == 8)
	{
		pcmToRealPointer = &ViPcmConverter<double>::pcmToReal8;
	}
	else if(sampleSize == 16)
	{
		pcmToRealPointer = &ViPcmConverter<double>::pcmToReal16;
	}
	else if(sampleSize == 24)
	{
		pcmToRealPointer = &ViPcmConverter<double>::pcmToReal24;
	}
	else if(sampleSize == 32)
	{
		pcmToRealPointer = &ViPcmConverter<double>::pcmToReal32;
	}

	mStream = buffer->createReadStream();
	mSpectrum = spectrum;
}

void ViSpectrumAnalyzerThread::setPositions(qint64 start, qint64 end)
{
	mStart = start;
	qint32 optimal = mBlockSize * (mFormat.sampleSize() / 8);
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

void ViSpectrumAnalyzerThread::setBlockSize(qint32 size)
{
	mBlockSize = size;
}

void ViSpectrumAnalyzerThread::setWindowFunction(QString functionName)
{
	mWindowFunction = functionName;
}

void ViSpectrumAnalyzerThread::run()
{
	qint32 index;
	qint32 sampleSize = mBlockSize;
	qint32 halfSize = sampleSize / 2;
	qint32 bytesSize = sampleSize * (mFormat.sampleSize() / 8);
	qint64 totalSize = mSizes.size() * bytesSize;
	
	char bytes[bytesSize];
	double samples[sampleSize];
	double fourier[sampleSize];

	ViFourierTransformer transformer;

	transformer.setWindowFunction(mWindowFunction);	transformer.setSize(mBlockSize);

	mSpectrum->initialize(halfSize + 1, mFormat.sampleRate() / 2);
	mStream->restart();

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
cout<<(totalSize - (mSizes.size() * bytesSize)) / (totalSize / 100.0)<<endl;
		emit changed((totalSize - (mSizes.size() * bytesSize)) / (totalSize / 100.0));
	}
	mSpectrum->finalize();
}

ViSpectrumAnalyzer::ViSpectrumAnalyzer(ViAudioBuffer *buffer)
	: QObject()
{
	setBlockSize(DEFAULT_BLOCK_SIZE);
	mBuffer = buffer;
	mThread.setData(buffer, &mSpectrum);
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mThread, SIGNAL(changed(qreal)), this, SIGNAL(changed(qreal)));
}

void ViSpectrumAnalyzer::analyze()
{
	mThread.quit();
	while(mThread.isRunning());
	mThread.setPositions(0, mBuffer->size());
	mThread.start();
}

void ViSpectrumAnalyzer::analyze(ViAudioPosition start, ViAudioPosition end)
{
	mThread.quit();
	while(mThread.isRunning());
	mThread.setPositions(start.position(ViAudioPosition::Bytes), end.position(ViAudioPosition::Bytes));
	mThread.start();
}

ViFloatSpectrum& ViSpectrumAnalyzer::spectrum()
{
	return mSpectrum;
}

void ViSpectrumAnalyzer::setBlockSize(qint32 size)
{
	mBlockSize = size;
	mThread.setBlockSize(mBlockSize);
}

qint32 ViSpectrumAnalyzer::blockSize()
{
	return mBlockSize;
}

void ViSpectrumAnalyzer::setWindowFunction(QString functionName)
{
	mThread.setWindowFunction(functionName);
}
