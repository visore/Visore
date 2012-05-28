#include "viqtconnection.h"

ViQtConnection::ViQtConnection()
	: ViAudioConnection()
{
}

ViFileInput* ViQtConnection::fileInput()
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViQtFileInput();
	}
	return mFileInput;
}

ViStreamInput* ViQtConnection::streamInput()
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViQtStreamInput();
	}
	return mStreamInput;
}

ViFileOutput* ViQtConnection::fileOutput()
{
	if(mFileOutput == NULL)
	{
		mFileOutput = new ViQtFileOutput();
	}
	return mFileOutput;
}

ViStreamOutput* ViQtConnection::streamOutput()
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViQtStreamOutput();
	}
	return mStreamOutput;
}
