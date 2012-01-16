#include "viaudioinput.h"

ViAudioInput::ViAudioInput(QAudioFormat format)
{
	mDevice = NULL;
	mFormat = format;
}

ViAudioInput::~ViAudioInput()
{
	if(mDevice != NULL)
	{
		mDevice->close();
		delete mDevice;
		mDevice = NULL;
	}
}

QIODevice* ViAudioInput::device()
{
	return mDevice;
}

QAudioFormat ViAudioInput::format()
{
	return mFormat;
}

qint64 ViAudioInput::size()
{
	return mDevice->size();
}
