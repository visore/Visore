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
		mFileInput = new ViQtFileInput();
		mFileInput->setBuffer(buffer);
		mFileInput->setFormat(format);
		mFileInput->setFile(filePath);
		mFileInput->initialize();
	}
	return mFileInput;
}

ViStreamInput* ViQtConnection::streamInput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device)
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViQtStreamInput();
		mStreamInput->setBuffer(buffer);
		mStreamInput->setFormat(format);
		mStreamInput->setDevice(device);
		mStreamInput->initialize();
	}
	return mStreamInput;
}

ViFileOutput* ViQtConnection::fileOutput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
{
	if(mFileOutput == NULL)
	{
		mFileOutput = new ViQtFileOutput();
		mFileOutput->setBuffer(buffer);
		mFileOutput->setFormat(format);
		mFileOutput->setFile(filePath);
		mFileOutput->initialize();
	}
	return mFileOutput;
}

ViStreamOutput* ViQtConnection::streamOutput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device)
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViQtStreamOutput();
		mStreamOutput->setBuffer(buffer);
		mStreamOutput->setFormat(format);
		mStreamOutput->setDevice(device);
		mStreamOutput->initialize();
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
