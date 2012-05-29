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
	LOG("Writing to file started.");
	mCoder.encode(mBuffer, mFilePath, mBuffer->format(), mFormat);
	emit started();
}

void ViFileOutput::stop()
{
	LOG("Writing to file stopped.");
	mCoder.stop();
	emit stopped();
}

void ViFileOutput::pause()
{
	LOG("Writing to file pasued.");
	emit paused();
}
