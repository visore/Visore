#include "vifileoutput.h"

ViFileOutput::ViFileOutput()
	: ViAudioOutput()
{
	mFilePath = "";
}

void ViFileOutput::setFile(QString filePath)
{
	mFilePath = filePath;
}

void ViFileOutput::bufferChanged(int size)
{
}

void ViFileOutput::initialize()
{
	ViAudioOutput::initialize();
}

void ViFileOutput::free()
{
	ViAudioOutput::free();
}
