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
	mCoder.decode(mFilePath, mBuffer, &mFormat);
	mBuffer->setFormat(mFormat);
	emit formatChanged(mFormat);
	emit started();
}

void ViFileInput::stop()
{
	mCoder.stop();
	emit stopped();
}

void ViFileInput::pause()
{
	emit paused();
}
