#include "viqtconnection.h"

ViQtConnection::ViQtConnection()
	: ViAudioConnection()
{
}

ViFileInput* ViQtConnection::fileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
{
	close(mFileInput);
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
	close(mStreamInput);
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
	close(mFileOutput);
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
	close(mStreamOutput);
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
