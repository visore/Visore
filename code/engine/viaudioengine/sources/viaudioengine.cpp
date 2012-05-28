#include "viaudioengine.h"

ViAudioEngine *ViAudioEngine::mEngine = NULL;

ViAudioEngine::ViAudioEngine()
	: ViSingleton()
{
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(progressed(short)), this, SIGNAL(spectrumProgressed(short)));
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(finished()), this, SIGNAL(spectrumFinished()));

	mFileInput = mConnection.fileInput();
	mFileOutput = mConnection.fileOutput();
	mStreamInput = mConnection.streamInput();
	mStreamOutput = mConnection.streamOutput();

	QObject::connect(&mProcessingChain, SIGNAL(changed()), this, SIGNAL(chainChanged()));
	mProcessingChain.setTransmission(mStreamOutput);
	mStreamOutput->setDevice(QAudioDeviceInfo::defaultOutputDevice());
	mStreamOutput->setFormat(ViAudioFormat::defaultFormat());

	mProcessingChain.attach(ViAudioConnection::Input, &mInputWaveFormer);
	mProcessingChain.attach(ViAudioConnection::Output, &mOutputWaveFormer);
}

ViAudioEngine::~ViAudioEngine()
{

}

ViAudioEngine* ViAudioEngine::instance()
{
	if(mEngine == NULL)
	{
		mEngine = new ViAudioEngine();
	}
	return mEngine;
}

ViRealSpectrum ViAudioEngine::spectrum()
{
	return mSpectrumAnalyzer.spectrum();
}

ViWaveForm& ViAudioEngine::wave(ViAudioConnection::Direction direction)
{
	if(direction == ViAudioConnection::Input)
	{
		return mInputWaveFormer.wave();
	}
	else if(direction == ViAudioConnection::Output)
	{
		return mOutputWaveFormer.wave();
	}
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
	mStreamInput->setFormat(ViAudioFormat::defaultFormat());
	mStreamInput->setDevice(QAudioDeviceInfo::defaultInputDevice());
	mProcessingChain.setTransmission(mStreamInput);
	mStreamInput->start();
}

void ViAudioEngine::stopRecording()
{
	mStreamInput->stop();
}

void ViAudioEngine::pauseRecording()
{
	mStreamInput->pause();
}

void ViAudioEngine::openFile(QString filePath)
{
	mFileInput->setFile(filePath);
	mProcessingChain.setTransmission(mFileInput);
	mFileInput->start();
}

void ViAudioEngine::saveFile(QString filePath)
{

}

void ViAudioEngine::calculateSpectrum(qint32 size, QString windowFunction)
{
	mExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	mExecutor.execute(mProcessingChain.buffer(ViAudioConnection::Input), &mSpectrumAnalyzer);
}
