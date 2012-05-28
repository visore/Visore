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
