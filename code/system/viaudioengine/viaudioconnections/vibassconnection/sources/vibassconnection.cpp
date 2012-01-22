#include "vibassconnection.h"

ViBassConnection::ViBassConnection()
	: ViAudioConnection()
{
	initialize();
}

ViBassConnection::~ViBassConnection()
{
	close();
}

ViFileInput* ViBassConnection::fileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViBassFileInput(buffer, metaData, filePath);
	}
	return mFileInput;
}

ViStreamInput* ViBassConnection::streamInput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViBassStreamInput(buffer, metaData, device);
	}
	return mStreamInput;
}

//ViFileOutput* ViBassConnection::fileOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, QString filePath);

ViStreamOutput* ViBassConnection::streamOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViBassStreamOutput(buffer, metaData, device);
	}
	return mStreamOutput;
}

void ViBassConnection::initialize()
{
	if(!BASS_Init(0, 44100, 0, 0, 0) || !BASS_Init(1, 44100, 0, 0, 0))
	{
		setErrorParameters("ViBassConnection - Initializing Error", "Could not initialize Bass resources", ViErrorInfo::Fatal);
	}
}

void ViBassConnection::close()
{
	if(!BASS_Free())
	{
		setErrorParameters("ViBassConnection - Closing Error", "Could not close Bass resources", ViErrorInfo::Fatal);
	}
}

QList<ViAudioDevice> ViBassConnection::devices(ViAudioDevice::ViAudioDeviceType type)
{
	return ViBassDevice::devices(type);
}

ViAudioDevice ViBassConnection::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{
	return ViBassDevice::defaultDevice(type);
}

/*******************************************
ViBassConnection
********************************************/

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioConnection* createConnection()
{
   return new ViBassConnection();
}

void deleteConnection(ViBassConnection *connection)
{
   delete connection;
}

#ifdef __cplusplus
}
#endif
