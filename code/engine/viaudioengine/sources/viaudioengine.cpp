#include "viaudioengine.h"
#include "vibassconnection.h"
#include "viqtconnection.h"

#include <QTimer>

ViAudioEngine::ViAudioEngine()
	: QObject()
{
	mStreamInput = NULL;
	mFileInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mInputType = ViAudioEngine::None;
/*
mAudioConnectionLoader = new ViLibrary<ViAudioConnection>();
if(!mAudioConnectionLoader->open(QCoreApplication::applicationDirPath()+"/engine/connections/libvibassconnection.so")) ViLogger::debug("Library cannot be loaded! ");
mAudioConnection = mAudioConnectionLoader->createObject("createConnection");
*/

mAudioConnection = new ViBassConnection();
ViAudioConnection *mAudioConnection2 = new ViQtConnection();



mMetaData = new ViAudioMetaData();
resetMetaData();
//mAudioInput = mAudioConnection->fileInput(mBuffer, metaData, "/home/visore/Desktop/a.wav");

ViAudioDevice outputDevice;
outputDevice.setId(-1);

//mFileOutput = mAudioConnection->fileOutput(mBuffer, metaData, "/home/visore/Desktop/testrec.mp3");

mProcessingChain = new ViAudioProcessingChain();



/*
mOriginalWaveFormer = new ViWaveFormer(mMetaData);
mCorrectedWaveFormer = new ViWaveFormer(mMetaData);

mProcessingChain->attachOriginalProcessor(mOriginalWaveFormer, ViProcessorList::Parallel);
ViObject::connectDirect(mOriginalWaveFormer, SIGNAL(completed(QSharedPointer<ViWaveFormChunk>)), this, SIGNAL(originalWaveChanged(QSharedPointer<ViWaveFormChunk>)));

mProcessingChain->attachCorrectedProcessor(mCorrectedWaveFormer, ViProcessorList::Parallel);
ViObject::connectDirect(mCorrectedWaveFormer, SIGNAL(completed(QSharedPointer<ViWaveFormChunk>)), this, SIGNAL(correctedWaveChanged(QSharedPointer<ViWaveFormChunk>)));
*/

//mOriginalSpectrumAnalyzer = new ViSpectrumAnalyzer();
//mOriginalSpectrumAnalyzer->setWindowFunction(ViWindowFunctionManager::selected("Hann Window Function"));
//ViObject::connectDirect(mOriginalWaveFormer, SIGNAL(completed(QSharedPointer<ViWaveFormChunk>)), mOriginalSpectrumAnalyzer, SLOT(start(QSharedPointer<ViWaveFormChunk>)));



//Make sure the file input is created before the stream input
mFileInput = mAudioConnection2->fileInput(mProcessingChain->originalBuffer(), mMetaData);
mStreamInput = mAudioConnection->streamInput(mProcessingChain->originalBuffer(), mMetaData);
mFileOutput = mAudioConnection->fileOutput(mProcessingChain->correctedBuffer(), mMetaData);
mStreamOutput = mAudioConnection->streamOutput(mProcessingChain->correctedBuffer(), mMetaData, &outputDevice);

//mProcessingChain->attachInput(mFileInput);
//mProcessingChain->attachInput(mStreamInput);
mProcessingChain->attachFileOutput(mFileOutput);
mProcessingChain->attachStreamOutput(mStreamOutput);

//ViWaveFormer *wave = new ViWaveFormer(mMetaData);
//mProcessingChain->attachOriginalProcessor(wave, ViProcessorList::Parallel);

ViObject::connectDirect(mStreamOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)));
ViObject::connectDirect(mProcessingChain->originalBuffer(), SIGNAL(changed(int)), this, SIGNAL(originalBufferChanged(int)));
ViObject::connectDirect(mProcessingChain->correctedBuffer(), SIGNAL(changed(int)), this, SIGNAL(correctedBufferChanged(int)));
//ViObject::connectDirect(wave, SIGNAL(completed(ViWaveFormChunk*)), this, SIGNAL(waveFormChanged(ViWaveFormChunk*)));






/*
mSongDetector = new ViSongDetector(mStreamOutput);
mSongDetector->setProxy(QNetworkProxy::HttpProxy, "137.215.6.53", 8080, "p04416376", "Rd28jRX");
mSongDetector->setKey("G1TZBE4IHJAYUSNCN");*/



mProcessingChain->attachOriginalProcessor(&mOriginalWaveSummarizer, ViProcessorList::Parallel);
mProcessingChain->attachCorrectedProcessor(&mCorrectedWaveSummarizer, ViProcessorList::Parallel);
ViObject::connect(&mOriginalWaveSummarizer, SIGNAL(changed()), this, SIGNAL(originalWaveChanged()));
ViObject::connect(&mCorrectedWaveSummarizer, SIGNAL(changed()), this, SIGNAL(correctedWaveChanged()));

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

ViAudioProcessingChain* ViAudioEngine::processingChain()
{
	return mProcessingChain;
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

void ViAudioEngine::setInput(ViAudioEngine::ViAudioType type)
{
	mInputType = type;
	emit inputChanged(mInputType);
}

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

int ViAudioEngine::volume()
{
	return mStreamOutput->volume() * 100;
}

void ViAudioEngine::setVolume(int volume)
{
	mStreamOutput->setVolume(volume / 100.0);
}

void ViAudioEngine::mute(bool value)
{
	mStreamOutput->mute(value);
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

void ViAudioEngine::setStreamPosition(ViAudioPosition position)
{
	mStreamOutput->setPosition(position);
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
	mMetaData->setFormat(ViFormatManager::selected("MP3"));
	mMetaData->setFrequency(44100);
	mMetaData->setChannels(2);
	mMetaData->setBitDepth(16);
}

ViWaveForm* ViAudioEngine::waveSummary(ViAudioBuffer::ViAudioBufferType type)
{
	if(type == ViAudioBuffer::Original)
	{
		return mOriginalWaveSummarizer.waveSummary();
	}
	else if(type == ViAudioBuffer::Corrected)
	{
		return mCorrectedWaveSummarizer.waveSummary();
	}
}
