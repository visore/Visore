#include "vifileoutput.h"
#include <QFile>

ViFileOutput::ViFileOutput()
	: ViAudioOutput()
{
	mFilePath = "";
	QObject::connect(&mCoder, SIGNAL(finished()), this, SIGNAL(finished()));
	setState(QAudio::IdleState);
}

void ViFileOutput::setFile(QString filePath)
{
	mFilePath = filePath;
}

ViAudioFormat ViFileOutput::format()
{
	return mFormat;
}

void ViFileOutput::setFormat(ViAudioFormat format)
{
	mFormat = format;
	emit formatChanged(format);
}

void ViFileOutput::start()
{
	LOG("Writing to file started.");
	//mCoder.encode(mBuffer, mFilePath, mBuffer->format(), mFormat);	
	setState(QAudio::ActiveState);
}

void ViFileOutput::stop()
{
	LOG("Writing to file stopped.");
	//mCoder.stop();
	setState(QAudio::StoppedState);
}

void ViFileOutput::pause()
{
	LOG("Writing to file pasued.");
	setState(QAudio::SuspendedState);
}
