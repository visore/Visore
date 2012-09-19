#include "viaudioengine.h"

#include "visampleenddetector.h"
#include "vifrequencyenddetector.h"

#include <tr1/functional>

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
	QObject::connect(&mProcessingChain, SIGNAL(progress(short)), this, SIGNAL(progress(short)));
	QObject::connect(&mProcessingChain, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));

	mProcessingChain.setTransmission(mStreamOutput);
	mStreamOutput->setDevice(QAudioDeviceInfo::defaultOutputDevice());
	mStreamOutput->setFormat(ViAudioFormat::defaultFormat());
	QObject::connect(mStreamOutput, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)), Qt::DirectConnection);
	QObject::connect(mStreamOutput, SIGNAL(lengthChanged(ViAudioPosition)), this, SIGNAL(lengthChanged(ViAudioPosition)), Qt::DirectConnection);

	mProcessingChain.setTransmission(mFileOutput);
	/*ViAudioFormat fileFormat = ViAudioFormat::defaultFormat();
	fileFormat.setCodec(ViAudioManager::codec("WAVE"));
	mFileOutput->setFormat(fileFormat);*/

	mStreamInput->setFormat(ViAudioFormat::defaultFormat());
	mStreamInput->setDevice(QAudioDeviceInfo::defaultInputDevice());

	mProcessingChain.attach(ViAudio::AudioInput, &mInputWaveFormer);
	mProcessingChain.attach(ViAudio::AudioOutput, &mOutputWaveFormer);

	//Spectrum analyzer
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), this, SIGNAL(spectrumChanged(ViRealSpectrum, qint64)), Qt::DirectConnection);
	mProcessingChain.attach(ViAudio::AudioOutput, &mSpectrumAnalyzer);
	
	//Song detector
	QObject::connect(&mSongDetector, SIGNAL(songDetected(ViSongInfo)), this, SIGNAL(songDetected(ViSongInfo)));
	QObject::connect(&mSongDetector, SIGNAL(songDetected(ViSongInfo)), mFileOutput, SLOT(setSongInfo(ViSongInfo)));
	mSongDetector.setKey("G1TZBE4IHJAYUSNCN");
	mProcessingChain.attach(ViAudio::AudioOutput, &mSongDetector);

	ViFrequencyEndDetector *endDetector = new ViFrequencyEndDetector();
	//QObject::connect(&mSpectrumAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), endDetector, SLOT(addSpectrum(ViRealSpectrum)), Qt::DirectConnection);
	mEndDetector = endDetector;
	//QObject::connect(mEndDetector, SIGNAL(songStarted(ViAudioPosition)), &mProcessingChain, SLOT(startInput()), Qt::DirectConnection);
	//QObject::connect(mEndDetector, SIGNAL(songEnded(ViAudioPosition)), &mProcessingChain, SLOT(endInput()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(songStarted(ViAudioPosition)), &mSongDetector, SLOT(enable()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(songEnded(ViAudioPosition)), &mSongDetector, SLOT(disable()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(songStarted(ViAudioPosition)), mFileOutput, SLOT(clearSongInfo()));
	
	QObject::connect(mEndDetector, SIGNAL(recordStarted(ViAudioPosition)), this, SIGNAL(recordStarted()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(recordEnded(ViAudioPosition)), this, SIGNAL(recordEnded()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(songStarted(ViAudioPosition)), this, SIGNAL(songStarted()), Qt::DirectConnection);
	QObject::connect(mEndDetector, SIGNAL(songEnded(ViAudioPosition)), this, SIGNAL(songEnded()), Qt::DirectConnection);

	mProcessingChain.attach(ViAudio::AudioInput, mEndDetector);

	//Project manager
	QObject::connect(&mProjectManager, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)), Qt::DirectConnection);
	QObject::connect(&mProjectManager, SIGNAL(started()), this, SIGNAL(progressStarted()), Qt::DirectConnection);
	QObject::connect(&mProjectManager, SIGNAL(finished()), this, SIGNAL(progressFinished()), Qt::DirectConnection);
}

ViAudioEngine::~ViAudioEngine()
{
	if(mEndDetector != NULL)
	{
		delete mEndDetector;
		mEndDetector = NULL;
	}
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
		//mProcessingChain.detach(mEndDetector);
		mProcessingChain.setTransmission(mFileInput);
	}
	else if(input == ViAudio::Line)
	{
		//mProcessingChain.attach(ViAudio::AudioInput, mEndDetector);
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

void ViAudioEngine::setPosition(int seconds)
{
	mStreamOutput->setPosition(seconds);
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
	/*mExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	if(!mExecutor.execute(mProcessingChain.buffer(ViAudio::AudioInput), &mSpectrumAnalyzer))
	{
		emit spectrumFinished();
	}*/
}

void ViAudioEngine::calculateCorrelation()
{
	/*mExecutor.setWindowSize(ViExecutor::defaultWindowSize());
	if(!mExecutor.execute(mProcessingChain.buffer(ViAudio::AudioInput), mProcessingChain.buffer(ViAudio::AudioOutput), &mCorrelator))
	{
		emit correlationFinished();
	}*/
}

void ViAudioEngine::startProject(QString name, QString filePath, ViAudioFormat format, short recordSides, bool play)
{
	mProjectManager.setProjectName(name);
	mProjectManager.setFilePath(filePath);
	mProjectManager.setSideCount(recordSides);
	mProjectManager.setPlayback(play);
	changeInput(ViAudio::Line);
	mProcessingChain.setProject(mProjectManager.project(), format);
	mProjectManager.start();
	startRecording();
}
