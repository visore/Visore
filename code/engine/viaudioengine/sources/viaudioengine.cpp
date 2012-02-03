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



mMetaData = new ViAudioMetaData();
resetMetaData();
//mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");


ViAudioDevice outputDevice;
outputDevice.setId(-1);

//mFileOutput = mAudioConnection->fileOutput(mBuffer, metaData, "/home/visore/Desktop/testrec.mp3");

mProcessingChain = new ViAudioProcessingChain();

//Make sure the file input is created before the stream input
mFileInput = mAudioConnection->fileInput(mProcessingChain->originalBuffer(), mMetaData);
mStreamInput = mAudioConnection->streamInput(mProcessingChain->originalBuffer(), mMetaData);
mFileOutput = mAudioConnection->fileOutput(mProcessingChain->correctedBuffer(), mMetaData);
mStreamOutput = mAudioConnection->streamOutput(mProcessingChain->correctedBuffer(), mMetaData, &outputDevice);

//mProcessingChain->attachInput(mFileInput);
//mProcessingChain->attachInput(mStreamInput);
mProcessingChain->attachFileOutput(mFileOutput);
mProcessingChain->attachStreamOutput(mStreamOutput);

ViWaveFormer *wave = new ViWaveFormer(mMetaData);
ViObject::connectDirect(wave, SIGNAL(completed(QList<double>)), this, SIGNAL(waveFormChanged(QList<double>)));
mProcessingChain->attachOriginalProcessor(wave, ViProcessorList::Parallel);

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
	resetMetaData();
	mFileInput->setFilePath(filePath);
	mProcessingChain->attachInput(mFileInput);

	//Required to ensure that the write metadata (aka bit depth is passed to Bass)
	mFileOutput->initialize();
	mStreamOutput->initialize();

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
	resetMetaData();
	mProcessingChain->attachInput(mStreamInput);

	//Required to ensure that the write metadata (aka bit depth is passed to Bass)
	mFileOutput->initialize();
	mStreamOutput->initialize();

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

void ViAudioEngine::resetMetaData()
{
	mMetaData->setFormat(ViFormatManager::format("MP3"));
	mMetaData->setFrequency(44100);
	mMetaData->setChannels(2);
	mMetaData->setBitDepth(16);
}
