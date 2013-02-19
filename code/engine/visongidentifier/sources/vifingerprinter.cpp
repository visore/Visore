#include <vifingerprinter.h>

ViFingerprinterThread::ViFingerprinterThread()
	: QThread()
{
	mFingerprint = "";
}

ViFingerprinterThread::~ViFingerprinterThread()
{
}

void ViFingerprinterThread::setBuffer(ViBuffer *buffer)
{
	mStream = buffer->createReadStream();
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
	if(mThread != NULL)
	{
		delete mThread;
		mThread = NULL;
	}
	emit generated();
}

QString ViFingerprinter::fingerprint()
{
	return mFingerprint;
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
