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
