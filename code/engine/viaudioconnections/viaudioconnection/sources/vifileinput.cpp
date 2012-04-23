#include "vifileinput.h"

ViFileInput::ViFileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
	: ViAudioInput(format, buffer)
{
	mFilePath = filePath;
}

void ViFileInput::setFilePath(QString filePath)
{
	mFilePath = filePath;
}
