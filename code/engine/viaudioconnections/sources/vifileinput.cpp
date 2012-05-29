#include "vifileinput.h"
#include <QFile>

ViFileInput::ViFileInput()
	: ViAudioInput()
{
	mFilePath = "";
}

void ViFileInput::setFile(QString filePath)
{
	mFilePath = filePath;
}

void ViFileInput::start()
{
	LOG("Reading file started.");
	mCoder.decode(mFilePath, mBuffer, &mFormat);
	mBuffer->setFormat(mFormat);
	emit formatChanged(mFormat);
	emit started();
}

void ViFileInput::stop()
{
	LOG("Reading file stopped.");
	mCoder.stop();
	emit stopped();
}

void ViFileInput::pause()
{
	LOG("Reading file paused.");
	emit paused();
}
