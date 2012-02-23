#include "vispectrumanalyzer.h"

void ViSpectrumAnalyzerThread::start()
{

}

void ViSpectrumAnalyzerThread::setWindowFunction(ViWindowFunction *windowFunction)
{
	mWindowFunction = windowFunction;
}

void ViSpectrumAnalyzerThread::addChunk(ViWaveFormChunk *chunk)
{
	mChunks.append(chunk);
	if(!isRunning())
	{
		start();
	}
}

void ViSpectrumAnalyzerThread::addWindow(qint64 size)
{
	mWindows.append(new ViSpectrumWindow(size, mWindowFunction));
}

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
{
	mThread = new ViSpectrumAnalyzerThread();
}

ViSpectrumAnalyzer::~ViSpectrumAnalyzer()
{
	if(mThread != NULL)
	{
		mThread->quit();
		delete mThread;
		mThread = NULL;
	}
}

void ViSpectrumAnalyzer::setWindowFunction(ViWindowFunction *windowFunction)
{
	mThread->setWindowFunction(windowFunction);
}

void ViSpectrumAnalyzer::start(ViWaveFormChunk *chunk)
{
	mThread->addChunk(chunk);
}

void ViSpectrumAnalyzer::stop()
{

}

bool ViSpectrumAnalyzer::isReady()
{

}
