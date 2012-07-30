#include "viaudioengine.h"

ViAudioEngine::ViAudioEngine()
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
	QObject::connect(&mProcessingChain, SIGNAL(buffering(short)), this, SIGNAL(buffering(short)));

	mProcessingChain.setTransmission(mStreamOutput);
	mStreamOutput->setDevice(QAudioDeviceInfo::defaultOutputDevice());
	mStreamOutput->setFormat(ViAudioFormat::defaultFormat());
	QObject::connect(mStreamOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)));

	mProcessingChain.setTransmission(mFileOutput);
	/*ViAudioFormat fileFormat = ViAudioFormat::defaultFormat();
	fileFormat.setCodec(ViAudioManager::codec("WAVE"));
	mFileOutput->setFormat(fileFormat);*/

	mStreamInput->setFormat(ViAudioFormat::defaultFormat());
	mStreamInput->setDevice(QAudioDeviceInfo::defaultInputDevice());

	mProcessingChain.attach(ViAudio::AudioInput, &mInputWaveFormer);
	mProcessingChain.attach(ViAudio::AudioOutput, &mOutputWaveFormer);

	//Spectrum analyzer
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), this, SIGNAL(spectrumChanged(ViRealSpectrum, qint64)));
	mProcessingChain.attach(ViAudio::AudioOutput, &mSpectrumAnalyzer);
	
	//Song detector
	QObject::connect(&mSongDetector, SIGNAL(songDetected(ViSongInfo)), this, SIGNAL(songDetected(ViSongInfo)));
	QObject::connect(&mSongDetector, SIGNAL(songDetected(ViSongInfo)), mFileOutput, SLOT(setSongInfo(ViSongInfo)));
	mSongDetector.setKey("G1TZBE4IHJAYUSNCN");
	mProcessingChain.attach(ViAudio::AudioOutput, &mSongDetector);

	QObject::connect(&mEndDetector, SIGNAL(songStarted(ViAudioPosition)), &mProcessingChain, SLOT(startInput(ViAudioPosition)), Qt::DirectConnection);
	QObject::connect(&mEndDetector, SIGNAL(songEnded(ViAudioPosition)), &mProcessingChain, SLOT(changeInput(ViAudioPosition)), Qt::DirectConnection);
	QObject::connect(&mEndDetector, SIGNAL(songStarted(ViAudioPosition)), &mSongDetector, SLOT(enable()), Qt::DirectConnection);
	QObject::connect(&mEndDetector, SIGNAL(songEnded(ViAudioPosition)), &mSongDetector, SLOT(disable()), Qt::DirectConnection);
}

ViAudioEngine::~ViAudioEngine()
{

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
		//mProcessingChain.detach(&mEndDetector);
		mProcessingChain.setTransmission(mFileInput);
	}
	else if(input == ViAudio::Line)
	{
		//mProcessingChain.attach(ViAudio::AudioInput, &mEndDetector);
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

void ViAudioEngine::startProject(QString name, QString filePath, ViAudioFormat format)
{
	mProcessingChain.setProject(filePath, format);
	changeInput(ViAudio::Line);
	startRecording();
}
