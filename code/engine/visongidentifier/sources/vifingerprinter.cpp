#include <vifingerprinter.h>

ViBufferOffsets::ViBufferOffsets()
{
	clear();
}

ViBufferOffsets::ViBufferOffsets(ViBuffer *buffer, int from, int to)
{
	mFrom = from;
	mTo = to;
	mBuffer = buffer;
}

ViBufferOffsets::ViBufferOffsets(const ViBufferOffsets &other)
{
	mFrom = other.mFrom;
	mTo = other.mTo;
	mBuffer = other.mBuffer;
}

int ViBufferOffsets::from()
{
	return mFrom;
}

int ViBufferOffsets::to()
{
	return mTo;
}

ViBuffer* ViBufferOffsets::buffer()
{
	return mBuffer;
}

void ViBufferOffsets::clear()
{
	mFrom = 0;
	mTo = 0;
	mBuffer = NULL;
}

bool ViBufferOffsets::isValid()
{
	return mBuffer != NULL && mTo > mFrom;
}


ViFingerprinterThread::ViFingerprinterThread()
	: QThread()
{
	mFingerprint = "";
}

ViFingerprinterThread::~ViFingerprinterThread()
{
}

void ViFingerprinterThread::setBufferOffset(ViBufferOffsets bufferOffset)
{
	mBufferOffset = bufferOffset;
	mStream = mBufferOffset.buffer()->createReadStream();
}

void ViFingerprinterThread::generate()
{
	start();
}

void ViFingerprinterThread::start()
{
	QThread::start();
}

QString ViFingerprinterThread::fingerprint()
{
	return mFingerprint;
}

void ViFingerprinterThread::setFingerprint(QString fingerprint)
{
	mFingerprint = fingerprint;
}

ViAudioPosition ViFingerprinterThread::duration()
{
	return mDuration;
}

void ViFingerprinterThread::setDuration(ViAudioPosition duration)
{
	mDuration = duration;
}

ViFingerprinter::ViFingerprinter()
	: QObject()
{
	mFingerprint = "";
	mThread = NULL;
}

ViFingerprinter::~ViFingerprinter()
{
	if(mThread != NULL)
	{
		delete mThread;
		mThread = NULL;
	}
}

void ViFingerprinter::finishFingerprint()
{
	mFingerprint = mThread->fingerprint();
	mDuration = mThread->duration();
	emit generated();
}

QString ViFingerprinter::fingerprint()
{
	return mFingerprint;
}

ViAudioPosition ViFingerprinter::duration()
{
	return mDuration;
}

void ViFingerprinter::setThread(ViFingerprinterThread *thread)
{
	if(mThread != NULL)
	{
		delete mThread;
	}
	mThread = thread;
	if(mThread != NULL)
	{
		QObject::connect(thread, SIGNAL(finished()), this, SLOT(finishFingerprint()));
	}
}

ViFingerprinterThread* ViFingerprinter::thread()
{
	return mThread;
}
