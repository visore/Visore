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

ViFileInput* ViAudioConnection::fileInput()
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViFileInput();
	}
	return mFileInput;
}

ViStreamInput* ViAudioConnection::streamInput()
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViStreamInput();
	}
	return mStreamInput;
}

ViFileOutput* ViAudioConnection::fileOutput()
{
	if(mFileOutput == NULL)
	{
		mFileOutput = new ViFileOutput();
	}
	return mFileOutput;
}

ViStreamOutput* ViAudioConnection::streamOutput()
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViStreamOutput();
	}
	return mStreamOutput;
}
