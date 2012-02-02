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
metaData->setChannels(2);

//mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");


ViAudioDevice outputDevice;
outputDevice.setId(-1);

//mFileOutput = mAudioConnection->fileOutput(mBuffer, metaData, "/home/visore/Desktop/testrec.mp3");

mProcessingChain = new ViAudioProcessingChain();

//Make sure the file input is created before the stream input
mFileInput = mAudioConnection->fileInput(mProcessingChain->originalBuffer(), metaData);
mStreamInput = mAudioConnection->streamInput(mProcessingChain->originalBuffer(), metaData);
mFileOutput = mAudioConnection->fileOutput(mProcessingChain->correctedBuffer(), metaData);
mStreamOutput = mAudioConnection->streamOutput(mProcessingChain->correctedBuffer(), metaData, &outputDevice);

mProcessingChain->attachInput(mFileInput);
mProcessingChain->attachInput(mStreamInput);
mProcessingChain->attachFileOutput(mFileOutput);
mProcessingChain->attachStreamOutput(mStreamOutput);

mProcessingChain->attachOriginalProcessor(new ViWaveFormer(), ViProcessorList::Parallel);

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

void ViAudioEngine::setInputFilePath(QString filePath)
{
	mFileInput->setFilePath(filePath);
	mFileInput->start();
}

void ViAudioEngine::setOutputFilePath(QString filePath)
{
	mFileOutput->setFilePath(filePath);
}

void ViAudioEngine::reset()
{
	mProcessingChain->reset();
}

void ViAudioEngine::startPlayback()
{
	mStreamOutput->start();
}

void ViAudioEngine::stopPlayback()
{
	mStreamOutput->stop();
}

void ViAudioEngine::pausePlayback()
{
	mStreamOutput->pause();
}

void ViAudioEngine::startRecording()
{
	mProcessingChain->reset();
	mStreamInput->start();
}

void ViAudioEngine::stopRecording()
{
	mStreamInput->stop();
}

void ViAudioEngine::setStreamPosition(qint64 position)
{
	mStreamOutput->setPosition(ViAudioTransmission::Milliseconds, position);
}

void ViAudioEngine::startOutputFile()
{
	mFileOutput->start();
}

void ViAudioEngine::stopOutputFile()
{
	mFileOutput->stop();
}
