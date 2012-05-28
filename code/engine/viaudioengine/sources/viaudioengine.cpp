#include "viaudioengine.h"

#include "viqtconnection.h"
#include "vimultiexecutor.h"

ViAudioEngine *ViAudioEngine::mEngine = NULL;

ViAudioEngine::ViAudioEngine()
	: ViSingleton()
{
	/*mProcessingChain.addOutput(ViAudioFormat::defaultFormat(), QAudioDeviceInfo::defaultOutputDevice());
	mProcessingChain.setInput(ViAudioFormat::defaultFormat(), "/home/visore/a.wav");

	ViWaveFormer *waveFormer1 = new ViWaveFormer();
	ViWaveFormer *waveFormer2 = new ViWaveFormer();
	QObject::connect(waveFormer1, SIGNAL(changed(ViWaveForm*)), this, SIGNAL(inputWaveChanged(ViWaveForm*)));
	QObject::connect(waveFormer2, SIGNAL(changed(ViWaveForm*)), this, SIGNAL(outputWaveChanged(ViWaveForm*)));
	mProcessingChain.attach(ViAudioConnection::Input, waveFormer1);
	mProcessingChain.attach(ViAudioConnection::Output, waveFormer2);

	mProcessingChain.start();*/

	QObject::connect(&mSpectrumAnalyzer, SIGNAL(progressed(short)), this, SIGNAL(spectrumChanged(short)));
	QObject::connect(&mSpectrumAnalyzer, SIGNAL(finished()), this, SIGNAL(spectrumFinished()));

	ViMultiExecutor *e = new ViMultiExecutor();
	ViQtConnection *c = new ViQtConnection();
	ib = new ViAudioBuffer();
	ViAudioBuffer *ob = new ViAudioBuffer();

	ViFileInput *si = c->fileInput(ViAudioFormat::defaultFormat(), ib, "/home/visore/a.wav");
	ViStreamOutput *so = c->streamOutput(ViAudioFormat::defaultFormat(), ob, QAudioDeviceInfo::defaultOutputDevice());

	e->setBuffer(ViAudioConnection::Input, ib);
	e->setBuffer(ViAudioConnection::Output, ob);

	si->start();
	so->start();

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
	mExecutor.setWindowSize(size);
	mSpectrumAnalyzer.setWindowFunction(windowFunction);
	mExecutor.execute(ib, &mSpectrumAnalyzer);
}
