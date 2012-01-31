#include "viaudioengine.h"
#include "vibassconnection.h"

#include <QTimer>

ViAudioEngine::ViAudioEngine()
	: QObject()
{
	mStreamInput = NULL;
	mFileInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
/*
mAudioConnectionLoader = new ViLibrary<ViAudioConnection>();
if(!mAudioConnectionLoader->open(QCoreApplication::applicationDirPath()+"/engine/connections/libvibassconnection.so")) ViLogger::debug("Library cannot be loaded! ");
mAudioConnection = mAudioConnectionLoader->createObject("createConnection");
*/

mAudioConnection = new ViBassConnection();



ViAudioMetaData *metaData = new ViAudioMetaData();
metaData->setFormat(ViFormatManager::format("MP3"));
metaData->setFrequency(44100);
metaData->setChannels(1);

//mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");


ViAudioDevice outputDevice;
outputDevice.setId(-1);

//mFileOutput = mAudioConnection->fileOutput(mBuffer, metaData, "/home/visore/Desktop/testrec.mp3");

mProcessingChain = new ViAudioProcessingChain();

mStreamInput = mAudioConnection->streamInput(mProcessingChain->originalBuffer(), metaData);
mFileInput = mAudioConnection->fileInput(mProcessingChain->originalBuffer(), metaData, "/home/visore/Desktop/a.wav");
mStreamOutput = mAudioConnection->streamOutput(mProcessingChain->originalBuffer(), metaData, &outputDevice);

mProcessingChain->attachInput(mStreamInput);
mProcessingChain->attachInput(mFileInput);
mProcessingChain->attachStreamOutput(mStreamOutput);

//mFileInput->start();

}

ViAudioEngine::~ViAudioEngine()
{
	/*if(mAudioConnectionLoader != NULL)
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
	}*/
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

void ViAudioEngine::startPlayback()
{
	if(mStreamOutput != NULL)
	{
		mStreamOutput->start();
	}
}

void ViAudioEngine::stopPlayback()
{
	if(mStreamOutput != NULL)
	{
		mStreamOutput->stop();
	}
}

void ViAudioEngine::pausePlayback()
{
	if(mStreamOutput != NULL)
	{
		mStreamOutput->pause();
	}
}

void ViAudioEngine::startRecording()
{
	if(mStreamInput != NULL)
	{
		mProcessingChain->reset();
		mStreamInput->start();
	}
}

void ViAudioEngine::stopRecording()
{
	if(mStreamInput != NULL)
	{
		mStreamInput->stop();
	}
}

void ViAudioEngine::setStreamPosition(qint64 position)
{
	if(mStreamInput != NULL)
	{
		mStreamOutput->setPosition(ViAudioTransmission::Milliseconds, position);
	}
}

void ViAudioEngine::startOutputFile()
{
	if(mFileOutput != NULL)
	{
		mFileOutput->start();
	}
}

void ViAudioEngine::stopOutputFile()
{
	if(mFileOutput != NULL)
	{
		mFileOutput->stop();
	}
}
