#include "vifileinput.h"

ViFileInput::ViFileInput()
	: ViAudioInput()
{
	mFilePath = "";
}

void ViFileInput::setFile(QString filePath)
{
	mFilePath = filePath;
}

void ViFileInput::initialize()
{
	ViAudioInput::initialize();
}

void ViFileInput::free()
{
	ViAudioInput::free();
}
