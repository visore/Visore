#include "viaudioengine.h"
#include "vibassconnection.h"

#include <QTimer>

ViAudioEngine::ViAudioEngine()
	: QObject()
{
	mBuffer = new ViAudioBuffer();
	mAudioInput = NULL;
/*
mAudioConnectionLoader = new ViLibrary<ViAudioConnection>();
if(!mAudioConnectionLoader->open(QCoreApplication::applicationDirPath()+"/engine/connections/libvibassconnection.so")) ViLogger::debug("Library cannot be loaded! ");
mAudioConnection = mAudioConnectionLoader->createObject("createConnection");
*/

mAudioConnection = new ViBassConnection();



ViAudioMetaData *metaData = new ViAudioMetaData();
metaData->setFormat(ViFormatManager::format("MP3"));

mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.aac");
//mAudioInput = mAudioConnection->streamInput(mBuffer, metaData);

ViAudioDevice outputDevice;
outputDevice.setId(-1);
//mAudioOutputs.append(mAudioConnection->streamOutput(mBuffer, metaData, &outputDevice));
mAudioOutputs.append(mAudioConnection->fileOutput(mBuffer, metaData, "/home/visore/Desktop/testrec.mp3"));

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
	for(int i = 0; i < mAudioOutputs.size(); ++i)
	{
		if(mAudioOutputs[i] != NULL)
		{
			mAudioOutputs[i]->start();
		}
	}
}

void ViAudioEngine::pauseOutput()
{
	for(int i = 0; i < mAudioOutputs.size(); ++i)
	{
		if(mAudioOutputs[i] != NULL)
		{
			mAudioOutputs[i]->pause();
		}
	}
}

void ViAudioEngine::stopOutput()
{
	for(int i = 0; i < mAudioOutputs.size(); ++i)
	{
		if(mAudioOutputs[i] != NULL)
		{
			mAudioOutputs[i]->stop();
		}
	}
}

void ViAudioEngine::setPosition(qint64 position)
{
	for(int i = 0; i < mAudioOutputs.size(); ++i)
	{
		if(mAudioOutputs[i] != NULL)
		{
			mAudioOutputs[i]->setPosition(ViAudioTransmission::Milliseconds, position);
		}
	}
}
