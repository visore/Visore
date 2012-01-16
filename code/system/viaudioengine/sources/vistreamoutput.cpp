#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput(ViAudioInput *input, QAudioDeviceInfo deviceInfo, QAudioFormat format)
	: ViAudioOutput(input, deviceInfo, format)
{
	setFormat(format);
}

ViStreamOutput::~ViStreamOutput()
{
	if(mAudioOutput != NULL)
	{
		disconnectOutput();
		delete mAudioOutput;
		mAudioOutput = NULL;
	}
}

void ViStreamOutput::start()
{
	//QTimer::singleShot(30000, this, SLOT(stopPlayback()));
	if(mAudioInput->device() != NULL)
	{
		mAudioOutput->start(mAudioInput->device());
	}
	else
	{
		ViLogger::debug("No data to output!");
	}
}

void ViStreamOutput::setFormat(QAudioFormat format)
{
	ViAudioOutput::setFormat(format);
	if(mAudioOutput != NULL)
	{
		disconnectOutput();
		delete mAudioOutput;
	}
	mAudioOutput = new QAudioOutput(mDeviceInfo, mFormat, this);
	connectOutput();
}

void ViStreamOutput::stopPlayback()
{
	ViLogger::debug("received ...9999999");
	mAudioOutput->stop();
	//outputFile.close();
}

qint64 ViStreamOutput::bufferLength()
{
	return audioLength(cBufferDurationUs);
}


