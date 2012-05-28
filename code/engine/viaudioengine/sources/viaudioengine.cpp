#include "viaudioengine.h"

#include "viqtconnection.h"
#include "vimultiexecutor.h"

ViAudioEngine *ViAudioEngine::mEngine = NULL;

ViAudioEngine::ViAudioEngine()
	: ViSingleton()
{
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(progressed(short)), this, SIGNAL(spectrumChanged(short)));
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(finished()), this, SIGNAL(spectrumFinished()));

	mConnection = new ViQtConnection();
	mFileInput = mConnection->fileInput();
	mFileOutput = mConnection->fileOutput();
	mStreamInput = mConnection->streamInput();
	mStreamOutput = mConnection->streamOutput();

	mProcessingChain.setTransmission(mFileInput);
	mProcessingChain.setTransmission(mStreamOutput);
	
	mFileInput->setFile("/home/visore/a.wav");
	mStreamOutput->setDevice(QAudioDeviceInfo::defaultOutputDevice());
	mStreamOutput->setFormat(ViAudioFormat::defaultFormat());

	mFileInput->start();
	mStreamOutput->start();

}

ViAudioEngine::~ViAudioEngine()
{
	delete mConnection;
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

void ViAudioEngine::calculateSpectrum(qint32 size, QString windowFunction)
{
	mExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	mExecutor.execute(mProcessingChain.buffer(ViAudioConnection::Input), &mSpectrumAnalyzer);
}
