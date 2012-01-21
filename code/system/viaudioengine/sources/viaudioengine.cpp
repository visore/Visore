#include "viaudioengine.h"



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
//ViAudioDevice inputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Input);
cout<<"dfgdfgd999"<<endl;
ViStreamInput *input = mAudioConnection->streamInput(mBuffer, metaData, NULL);
//ViFileInput *input = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");
ViStreamOutput *output = mAudioConnection->streamOutput(mBuffer, metaData, NULL);
//ViStreamOutput *output = mAudioConnection->streamOutput(mBuffer, metaData, &outputDevice);
input->start();
output->start();









/*
ViAudioMetaData *metaData = new ViAudioMetaData();

mAudioConnectionLoader = new ViLibrary<ViAudioConnection>(QCoreApplication::applicationDirPath()+"/system/audioengine/audioconnections/libvibassconnection.so");
if(!mAudioConnectionLoader->open()) ViLogger::debug("Library cannot be loaded! ");
mAudioConnection = mAudioConnectionLoader->createObject("createConnection");

ViAudioDevice inputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Input);
*/

/*
ViLibrary<ViBassStreamInput> *m = new ViLibrary<ViBassStreamInput>(QCoreApplication::applicationDirPath()+"/system/audioengine/audioconnections/libvibassconnection.so");
if(!m->open()) ViLogger::debug("Library cannot be loaded! ");
ViBassStreamInput *ii = m->createObject("createC");*/
//ii->start();
//ViAudioDevice outputDevice = mAudioConnection->defaultDevice(ViAudioDevice::Output);

//ViFileInput *input = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");
//ViStreamInput *input = mAudioConnection->streamInput(mBuffer, metaData, &inputDevice);
//ViStreamOutput *output = mAudioConnection->streamOutput(mBuffer, metaData, &outputDevice);

//input->start();
//output->start();

/*ViAudioMetaData *metaData = new ViAudioMetaData();
ViBassStreamInput *input = new ViBassStreamInput(mBuffer, metaData, 0);

input->start();
*/


//BASS_Free();








//ToDo: Make sure metadata is set beofre conitueng - eg: signal
/*output->setDevice(ViAudioDevice::defaultDevice(ViAudioDevice::Output));*/
//output->setMetaData(input->metaData());
//output->start();

/*ViLibrary<ViFileInput> loader(QCoreApplication::applicationDirPath()+"/system/connections/libvibassconnection.so");
if(!loader.open()) ViLogger::debug("Library cannot be loaded! ");
ViFileInput *input = loader.create("createFileInput"mBuffer, "/home/visore/Desktop/a.wav");*/
////input->start();






















	//Test
	
	/*QAudioFormat format;
	// set up the format you want, eg.
WavFile w;
w.open("/home/visore/Desktop/a.wav");
format = w.fileFormat();

	format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(format)) {
		format = info.nearestFormat(format);
ViLogger::debug("999999999998888888888");
		}
*/
	//initializeInputStream(info, format);
	//initializeInputFile("/home/visore/Desktop/test.raw");
//	initializeOutputStream();
//	mAudioInput->start();
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
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
		mAudioInput = NULL;
	}
	if(mAudioOutputs.size() != 0)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			delete mAudioOutputs[i];
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

void ViAudioEngine::changeReceived(int startIndex, int size)
{
	//ViLogger::debug("Data received: index "+QString::number(startIndex)+" size "+QString::number(size));
	//ViLogger::debug("yyyyy: "+QString::number(mBuffer->size()));
	//mAudioOutputs[0]->start();
}

