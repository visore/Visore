#include "vifileoutput.h"

ViFileOutput::ViFileOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViAudioOutput(buffer, metaData)
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
