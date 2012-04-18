#include "viqtconnection.h"

ViQtConnection::ViQtConnection()
	: ViAudioConnection()
{
	initialize();
}

ViQtConnection::~ViQtConnection()
{
	close();
}

ViFileInput* ViQtConnection::fileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViQtFileInput(buffer, metaData, filePath);
	}
	return mFileInput;
}

ViStreamInput* ViQtConnection::streamInput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViQtStreamInput(buffer, metaData, device);
	}
	return mStreamInput;
}

ViFileOutput* ViQtConnection::fileOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, QString filePath)
{

}

ViStreamOutput* ViQtConnection::streamOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{

}

void ViQtConnection::initialize()
{
	mStreamInput = NULL;
	mFileInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
}

void ViQtConnection::close()
{

}

void ViQtConnection::populateInputFormats()
{

}

void ViQtConnection::populateOutputFormats()
{

}

QList<ViAudioDevice> ViQtConnection::devices(ViAudioDevice::ViAudioDeviceType type)
{

}

ViAudioDevice ViQtConnection::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{

}

/*******************************************
ViQtConnection
********************************************/

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioConnection* createConnection()
{
   return new ViQtConnection();
}

void deleteConnection(ViQtConnection *connection)
{
   delete connection;
}

#ifdef __cplusplus
}
#endif
