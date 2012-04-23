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

ViFileInput* ViQtConnection::fileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViQtFileInput(format, buffer, filePath);
	}
	return mFileInput;
}

ViStreamInput* ViQtConnection::streamInput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViQtStreamInput(format, buffer, device);
	}
	return mStreamInput;
}

ViFileOutput* ViQtConnection::fileOutput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
{
	if(mFileOutput == NULL)
	{
		mFileOutput = new ViQtFileOutput(format, buffer, filePath);
	}
	return mFileOutput;
}

ViStreamOutput* ViQtConnection::streamOutput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViQtStreamOutput(format, buffer, device);
	}
	return mStreamOutput;
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
