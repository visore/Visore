#include "vifileoutput.h"
#include <QFile>

ViFileOutput::ViFileOutput()
	: ViAudioOutput()
{
	mFilePath = "";
}

void ViFileOutput::setFile(QString filePath)
{
	mFilePath = filePath;
}

void ViFileOutput::start()
{
	mCoder.encode(mBuffer, mFilePath, mBuffer->format(), mFormat);
	emit started();
}

void ViFileOutput::stop()
{
	mCoder.stop();
	emit stopped();
}

void ViFileOutput::pause()
{
	emit paused();
}
