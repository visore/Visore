#include "viaudioengine.h"

#include <QTimer>

ViAudioEngine::ViAudioEngine()
	: QObject()
{
	mBuffer = new ViAudioBuffer();
	mAudioInput = NULL;






/*

cout<<"gfdfgdfgdfgdfg1000000000: "<<endl;
	if(!BASS_RecordInit(-1))
	{
		//setErrorParameters("ViBassStreamInput - Initializing Error", "Could not initialize the Bass recording device", ViErrorInfo::Fatal);
		//return;
cout<<"info999999999999: "<<endl;	
	}

   BASS_DEVICEINFO info;
DWORD a = BASS_RecordGetDevice();
if(a < 0)cout<<"error 1"<<endl;	
if(!BASS_RecordGetDeviceInfo(0, &info))cout<<"error 2"<<endl;	


cout<<"gfdfgdfgdfgdfg111111111111: "<<BASS_ErrorGetCode()<<endl;	
cout<<"info: "<<info.driver<<endl;	



	HRECORD recordHandle = BASS_RecordStart(44100, 2, 0, 0, 0);
	if(!recordHandle)
	{
		//mParent->setErrorParameters("ViBassStreamInput - Stream Input Error", "The stream could not be opened", ViErrorInfo::Fatal);
		//return;
	}
cout<<"infox: "<<BASS_ErrorGetCode()<<endl;












	/*format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);*/

mAudioConnectionLoader = new ViLibrary<ViAudioConnection>(QCoreApplication::applicationDirPath()+"/system/audioengine/audioconnections/libvibassconnection.so");
if(!mAudioConnectionLoader->open()) ViLogger::debug("Library cannot be loaded! ");
mAudioConnection = mAudioConnectionLoader->createObject("createConnection");
/*mAudioConnection = new ViBassConnection();
ViAudioMetaData *metaData = new ViAudioMetaData();
ViAudioDevice inputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Input);
ViStreamInput *input = new ViBassStreamInput(mBuffer, metaData, NULL);
input->start();*/



ViAudioMetaData *metaData = new ViAudioMetaData();

//mAudioConnection = new ViBassConnection();
cout<<"dfgdfgd999"<<endl;
mAudioInput = mAudioConnection->streamInput(mBuffer, metaData, NULL);
//mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");
//ViFileInput *input = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");
/*
ViLogger::debug("frequency: "+metaData->frequency());
ViLogger::debug("channels: "+metaData->channels());
ViLogger::debug("seconds: "+metaData->seconds());
ViLogger::debug("bytes: "+metaData->bytes());
ViLogger::debug("level right: "+metaData->level().right());
ViLogger::debug("level left: "+metaData->level().left());*/
//ViAudioDevice outputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Output);
ViAudioDevice outputDevice;
outputDevice.setId(-1);
mAudioOutputs.append(mAudioConnection->streamOutput(mBuffer, metaData, &outputDevice));

//ViFileInput *input = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");
//ViStreamOutput *output = mAudioConnection->streamOutput(mBuffer, metaData, &outputDevice);
//input->start();
//ViStreamOutput *output = mAudioConnection->streamOutput(mBuffer, metaData, NULL);
//output->start();



}

ViAudioEngine::~ViAudioEngine()
{
	if(mAudioConnectionLoader != NULL)
	{
		mAudioConnectionLoader->deleteObject("deleteConnection", mAudioConnection);
		delete mAudioConnectionLoader;
		mAudioConnectionLoader = NULL;
	}
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
		mAudioInput = NULL;
	}
	if(mAudioOutputs.size() != 0)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			ViObject::disconnect(mAudioOutputs[i], SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
			mAudioOutputs[i] = NULL;
		}
	}
	if(mBuffer != NULL)
	{
		delete mBuffer;
		mBuffer = NULL;
	}
}
/*
void ViAudioEngine::initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format)
{
	if(mAudioInput != NULL)
	{
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
	}
	mInputAudioType = ViAudioEngine::Stream;
	//mAudioInput = new ViStreamInput(mBuffer, deviceInfo, format);
	ViObject::connect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViAudioEngine::initializeInputFile(QString filePath)
{
	if(mAudioInput != NULL)
	{
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
	}
	mInputAudioType = ViAudioEngine::File;
	//mAudioInput = new ViFileInput(mBuffer, filePath);
	ViObject::connect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViAudioEngine::initializeOutputStream()
{
	if(mOutputAudioType == ViAudioEngine::File)
	{
		mOutputAudioType = ViAudioEngine::FileAndStream;
	}
	else if(mOutputAudioType == ViAudioEngine::None)
	{
		mOutputAudioType = ViAudioEngine::Stream;
	}
	//mAudioOutputs.append(new ViStreamOutput(mBuffer));
}

void ViAudioEngine::initializeOutputFile()
{
	if(mOutputAudioType == ViAudioEngine::Stream)
	{
		mOutputAudioType = ViAudioEngine::FileAndStream;
	}
	else if(mOutputAudioType == ViAudioEngine::None)
	{
		mOutputAudioType = ViAudioEngine::File;
	}
	//mAudioOutputs.append(new ViStreamOutput(mBuffer));
}*/

void ViAudioEngine::startInput()
{
	if(mAudioInput != NULL)
	{
		mAudioInput->start();
	}
}

void ViAudioEngine::pauseInput()
{
	if(mAudioInput != NULL)
	{
		mAudioInput->pause();
	}
}

void ViAudioEngine::stopInput()
{
	if(mAudioInput != NULL)
	{
		mAudioInput->stop();
	}
}

void ViAudioEngine::startOutput()
{
	if(mAudioInput != NULL)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			mAudioOutputs[i]->start();
		}
	}
}

void ViAudioEngine::pauseOutput()
{
	if(mAudioInput != NULL)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			mAudioOutputs[i]->pause();
		}
	}
}

void ViAudioEngine::stopOutput()
{
	if(mAudioInput != NULL)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			mAudioOutputs[i]->stop();
		}
	}
}
