#include "viaudioconnection.h"

ViAudioConnection::ViAudioConnection()
{
	mFileInput = NULL;
	mStreamInput = NULL;
	mFileOutput = NULL;
	mStreamOutput = NULL;
}

ViAudioConnection::~ViAudioConnection()
{
	if(mFileInput != NULL)
	{
		delete mFileInput;
		mFileInput = NULL;
	}
	if(mStreamInput != NULL)
	{
		delete mStreamInput;
		mStreamInput = NULL;
	}
	if(mFileOutput != NULL)
	{
		delete mFileOutput;
		mFileOutput = NULL;
	}
	if(mStreamOutput != NULL)
	{
		delete mStreamOutput;
		mStreamOutput = NULL;
	}
}

QList<ViAudioFormat*> ViAudioConnection::supportedInputFormats()
{
	return mInputFormats;
}

QList<ViAudioFormat*> ViAudioConnection::supportedOutputFormats()
{
	return mOutputFormats;
}
