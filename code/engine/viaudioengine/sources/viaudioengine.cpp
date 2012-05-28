#include "viaudioengine.h"

ViAudioEngine *ViAudioEngine::mEngine = NULL;

ViAudioEngine::ViAudioEngine()
	: ViSingleton()
{
	mProcessingChain.addOutput(ViAudioFormat::defaultFormat(), QAudioDeviceInfo::defaultOutputDevice());
	mProcessingChain.setInput(ViAudioFormat::defaultFormat(), "/home/visore/a.wav");

	ViWaveFormer *waveFormer1 = new ViWaveFormer();
	ViWaveFormer *waveFormer2 = new ViWaveFormer();
	QObject::connect(waveFormer1, SIGNAL(changed(ViWaveForm*)), this, SIGNAL(inputWaveChanged(ViWaveForm*)));
	QObject::connect(waveFormer2, SIGNAL(changed(ViWaveForm*)), this, SIGNAL(outputWaveChanged(ViWaveForm*)));
	mProcessingChain.attach(ViAudioConnection::Input, waveFormer1);
	mProcessingChain.attach(ViAudioConnection::Output, waveFormer2);

	mProcessingChain.start();

	QObject::connect(&mSpectrumAnalyzer, SIGNAL(progressed(short)), this, SIGNAL(spectrumChanged(short)));
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(finished()), this, SIGNAL(spectrumFinished()));
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

void ViAudioEngine::calculateSpectrum(qint32 size, QString windowFunction)
{
	mProcessorExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	mProcessorExecutor.execute(&mSpectrumAnalyzer, mProcessingChain.buffer(ViAudioConnection::Input));
}
