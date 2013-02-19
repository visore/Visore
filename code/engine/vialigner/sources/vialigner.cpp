#include <vialigner.h>
#include <vipowercalculator.h>
#include <vipcmconverter.h>
#include <vilogger.h>

#define MAXIMUM_DELAY 524288 //262144

ViAlignerThread::ViAlignerThread()
	: QThread()
{
	mAligner = NULL;
}

void ViAlignerThread::setAligner(ViAligner *aligner)
{
	mAligner = aligner;
	QObject::connect(this, SIGNAL(finished()), mAligner, SLOT(alignBuffer()));
}

void ViAlignerThread::run()
{
	mAligner->execute();
}

ViAligner::ViAligner()
	: QObject()
{
	setMaximumDelay(MAXIMUM_DELAY);
	mDelay = 0;
	mThread.setAligner(this);
}

void ViAligner::alignBuffer()
{
	if(mDelay == 0)
	{
		LOG("The buffers are already aligned.");
	}
	else if(mDelay < 0)
	{
		mBuffer1->remove(0, qAbs(delay().bytes()));
		LOG("A positive delay of " + QString::number(qAbs(mDelay)) + " samples was detected.");
	}
	else
	{
		mBuffer2->remove(0, qAbs(delay().bytes()));
		LOG("A negative delay of " + QString::number(qAbs(mDelay)) + " samples was detected.");
	}

	//Make buffers the same length
	qint64 size1 = mBuffer1->size();
	qint64 size2 = mBuffer2->size();
	if(size1 > size2)
	{
		mBuffer1->remove(size2, size1 - size2);
		LOG("The first buffer was reduced by " + QString::number(size1 - size2) + " samples.");
	}
	else if(size2 > size1)
	{
		mBuffer2->remove(size1, size2 - size1);
		LOG("The second buffer was reduced by " + QString::number(size2 - size1) + " samples.");
	}

	mChunk1.clear();
	mChunk2.clear();
	mBuffer1 = NULL;
	mBuffer2 = NULL;

	emit progressed(100);
	emit finished();
}
		
qint64 ViAligner::setMaximumDelay(qint64 samples)
{
	mMaximumDelay = ViPowerCalculator::nextPower(samples, 2);
	return mMaximumDelay;
}

ViAudioPosition ViAligner::setMaximumDelay(ViAudioPosition delay)
{
	delay.setSamples(setMaximumDelay(delay.samples()));
	return delay;
}

ViAudioPosition ViAligner::delay()
{
	if(mDelay < 0)
	{
		return ViAudioPosition(mDelay, ViAudioPosition::Samples, mFormat1);
	}
	else
	{
		return ViAudioPosition(mDelay, ViAudioPosition::Samples, mFormat2);
	}
}

qint64 ViAligner::setDelay(qint64 delay)
{
	ViAudioFormat *format;
	if(mDelay < 0)
	{
		format = &mFormat1;
	}
	else
	{
		format = &mFormat2;
	}
	if(delay % 2 != 0 && format->channelCount() % 2 == 0) // Ensures that the channels are not switched
	{
		--delay;
	}
	mDelay = delay;
	return mDelay;
}

void ViAligner::align(ViBuffer *buffer1, ViBuffer *buffer2, int maximumDelay)
{
	emit started();
	emit progressed(0);

	mChunk1.clear();
	mChunk2.clear();

	mBuffer1 = buffer1;
	mBuffer2 = buffer2;
	if(mBuffer1 == NULL || mBuffer2 == NULL)
	{
		LOG("One of the buffers is not available. Two are needed for alignment.");
		emit progressed(100);
		emit finished();
		return;
	}

	mFormat1 = mBuffer1->format();
	mFormat2 = mBuffer2->format();
	if(!mFormat1.isValid() || !mFormat2.isValid())
	{
		LOG("Invalid audio format. Can't align the buffers.");
		emit progressed(100);
		emit finished();
		return;
	}

	if(maximumDelay > 0)
	{
		setMaximumDelay(maximumDelay);
	}
	if(mBuffer1->size() < maximumDelay || mBuffer2->size() < maximumDelay)
	{
		LOG("The buffers don't have enough data for alignment. Try reducing the maximum delay.");
		emit progressed(100);
		emit finished();
		return;
	}

	mDelay = 0;
	mChunk1.resize(mMaximumDelay);
	mChunk2.resize(mMaximumDelay);

	ViBufferStreamPointer stream1 = mBuffer1->createReadStream();
	ViBufferStreamPointer stream2 = mBuffer2->createReadStream();
	
	//Read from the middle of the file, the start is often too similar
	int size = stream1->size();
	if(size > stream2->size())
	{
		size = stream2->size();
	}
	int position = size / 2;
	if(position < mMaximumDelay)
	{
		position = size - mMaximumDelay;
	}
	stream1->setPosition(position);
	stream2->setPosition(position);

	ViPcmConverter<qreal> converter(mFormat1.sampleSize());
	int bytes = ViAudioPosition::convertPosition(mMaximumDelay, ViAudioPosition::Samples, ViAudioPosition::Bytes, mFormat1);
	ViBufferChunk *bufferChunk = new ViBufferChunk(bytes);
	stream1->read(*bufferChunk);
	converter.pcmToReal(bufferChunk->data(), mChunk1.data(), bufferChunk->size());
	delete bufferChunk;
	
	converter.setSize(mFormat2.sampleSize());
	bytes = ViAudioPosition::convertPosition(mMaximumDelay, ViAudioPosition::Samples, ViAudioPosition::Bytes, mFormat2);
	bufferChunk = new ViBufferChunk(bytes);
	stream2->read(*bufferChunk);
	converter.pcmToReal(bufferChunk->data(), mChunk2.data(), bufferChunk->size());
	delete bufferChunk;

	if(mChunk1.size() != mChunk2.size())
	{
		LOG("The length of the two chunks are not the same. Cannot align buffers.");
		mChunk1.clear();
		mChunk2.clear();
		emit progressed(100);
		emit finished();
		return;
	}

	mThread.start();
}
