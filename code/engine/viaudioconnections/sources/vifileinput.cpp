#include "vifileinput.h"
#include <QFile>

ViFileInput::ViFileInput()
	: ViAudioInput()
{
	mFilePath = "";
	QObject::connect(&mCoder, SIGNAL(finished()), this, SIGNAL(finished()));
	setState(QAudio::IdleState);
}

void ViFileInput::setFile(QString filePath)
{
	mFilePath = filePath;
}

ViAudioFormat ViFileInput::format()
{
	mBuffer->format();
}

void ViFileInput::start()
{
	LOG("Reading file started.");
	mCoder.disconnect();
	QObject::connect(&mCoder, SIGNAL(formatChanged(ViAudioFormat)), mBuffer, SLOT(setFormat(ViAudioFormat)));
	mCoder.decode(mFilePath, mBuffer);
	setState(QAudio::ActiveState);
}

void ViFileInput::stop()
{
	LOG("Reading file stopped.");
	//mCoder.stop();
	setState(QAudio::StoppedState);
}

void ViFileInput::pause()
{
	LOG("Reading file paused.");
	setState(QAudio::SuspendedState);
}
