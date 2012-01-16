#include "vistreaminput.h"

ViStreamInput::ViStreamInput(QIODevice *device, QAudioDeviceInfo deviceInfo, QAudioFormat format)
{
	mAudioInput = new QAudioInput(deviceInfo, format, this);
	mDevice = device;
}

ViStreamInput::~ViStreamInput()
{
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
		mAudioInput = NULL;
	}
}

void ViStreamInput::start()
{
	QTimer::singleShot(30000, this, SLOT(stopRecording()));
	mAudioInput->start(mDevice);
	//ViObject::connect(mIODevice, SIGNAL(readyRead()), this, SLOT(dataReady()));
}

qint64 ViStreamInput::read(char *data, qint64 maxSize)
{
	return mDevice->read(data, maxSize);
}

void ViStreamInput::dataReady()
{
	ViLogger::debug("received ...");
	//emit notify();
}

void ViStreamInput::stopRecording()
{
	ViLogger::debug("received 30s of data");
	mAudioInput->stop();
	//outputFile.close();
}


