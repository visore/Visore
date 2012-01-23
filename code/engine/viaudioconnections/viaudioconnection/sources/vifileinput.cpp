#include "vifileinput.h"

ViFileInput::ViFileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViAudioInput(buffer, metaData)
{
	mFilePath = filePath;
}

void ViFileInput::setFilePath(QString filePath)
{
	mFilePath = filePath;
}
