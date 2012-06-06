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
	close(mFileInput);
	close(mStreamInput);
	close(mFileOutput);
	close(mStreamOutput);
}

void ViAudioConnection::close(ViAudioTransmission *transmission)
{
	if(transmission != NULL)
	{
		delete transmission;
		transmission = NULL;
	}
}
