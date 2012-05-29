#include "viaudioengine.h"

ViAudioEngine *ViAudioEngine::mEngine = NULL;

ViAudioEngine::ViAudioEngine()
	: ViSingleton()
{
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(progressed(short)), this, SIGNAL(spectrumProgressed(short)));
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(finished()), this, SIGNAL(spectrumFinished()));

	QObject::connect(&mCorrelator, SIGNAL(progressed(short)), this, SIGNAL(correlationProgressed(short)));
	QObject::connect(&mCorrelator, SIGNAL(finished()), this, SIGNAL(correlationFinished()));

	mFileInput = mConnection.fileInput();
	mFileOutput = mConnection.fileOutput();
	mStreamInput = mConnection.streamInput();
	mStreamOutput = mConnection.streamOutput();

	QObject::connect(&mProcessingChain, SIGNAL(changed()), this, SIGNAL(chainChanged()));

	mProcessingChain.setTransmission(mStreamOutput);
	mStreamOutput->setDevice(QAudioDeviceInfo::defaultOutputDevice());
	mStreamOutput->setFormat(ViAudioFormat::defaultFormat());
	QObject::connect(mStreamOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)));

	mStreamInput->setFormat(ViAudioFormat::defaultFormat());
	mStreamInput->setDevice(QAudioDeviceInfo::defaultInputDevice());

	mProcessingChain.attach(ViAudio::AudioInput, &mInputWaveFormer);
	mProcessingChain.attach(ViAudio::AudioOutput, &mOutputWaveFormer);
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

ViWaveForm& ViAudioEngine::wave(ViAudio::Mode mode)
{
	if(mode == ViAudio::AudioInput)
	{
		return mInputWaveFormer.wave();
	}
	else if(mode == ViAudio::AudioOutput)
	{
		return mOutputWaveFormer.wave();
	}
}

ViCorrelationResult& ViAudioEngine::correlation()
{
	mCorrelator.result();
}

void ViAudioEngine::changeInput(ViAudio::Input input)
{
	if(input == ViAudio::File)
	{
		mProcessingChain.setTransmission(mFileInput);
	}
	else if(input == ViAudio::Line)
	{
		mProcessingChain.setTransmission(mStreamInput);
	}
	emit inputChanged(input);
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
	mFileInput->start();
}

void ViAudioEngine::saveFile(QString filePath)
{

}

void ViAudioEngine::calculateSpectrum(qint32 size, QString windowFunction)
{
	mExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	if(!mExecutor.execute(mProcessingChain.buffer(ViAudio::AudioInput), &mSpectrumAnalyzer))
	{
		emit spectrumFinished();
	}
}

void ViAudioEngine::calculateCorrelation()
{
	mExecutor.setWindowSize(ViExecutor::defaultWindowSize());
	if(!mExecutor.execute(mProcessingChain.buffer(ViAudio::AudioInput), mProcessingChain.buffer(ViAudio::AudioOutput), &mCorrelator))
	{
		emit correlationFinished();
	}
}
