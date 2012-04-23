#include "vifileoutput.h"

ViFileOutput::ViFileOutput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
	: ViAudioOutput(format, buffer)
{
	mFilePath = filePath;
}

void ViFileOutput::setFilePath(QString filePath)
{
	mFilePath = filePath;
}

bool ViFileOutput::setPosition(ViAudioPosition position)
{
	return false;
}

ViAudioPosition ViFileOutput::position()
{
	//return ViAudioPosition(0, 0, 0);
}

void ViFileOutput::bufferChanged(int size)
{
}
