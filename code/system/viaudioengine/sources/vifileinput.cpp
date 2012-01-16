#include "vifileinput.h"

ViFileInput::ViFileInput(QString filePath, QAudioFormat format)
	: ViAudioInput(format)
{
	mDevice = new QFile(filePath);
	mAnalysisFile = new QFile(filePath);
	mDevice->open(QIODevice::ReadOnly);
	mAnalysisFile->open(QIODevice::ReadOnly);
}

ViFileInput::~ViFileInput()
{
	if(mAnalysisFile != NULL)
	{
		mAnalysisFile->close();
		delete mAnalysisFile;
		mAnalysisFile = NULL;
	}
}
