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

qint64 ViFileOutput::setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position)
{
	return -1;
}

qint64 ViFileOutput::position(ViAudioTransmission::ViTransmissionType type)
{
	return -1;
}

void ViFileOutput::bufferChanged(int startIndex, int size)
{
}
