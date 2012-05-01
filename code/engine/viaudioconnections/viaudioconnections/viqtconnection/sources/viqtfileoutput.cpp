#include "viqtfileoutput.h"

ViQtFileOutput::ViQtFileOutput()
	: ViFileOutput()
{
}

ViQtFileOutput::~ViQtFileOutput()
{
	free();
}

void ViQtFileOutput::initialize()
{
	ViFileOutput::initialize();
}

void ViQtFileOutput::free()
{
	ViFileOutput::free();
}

void ViQtFileOutput::start()
{
	
	ViAudioFormat format;
	format.setChannelCount(2);
	format.setSampleRate(44100);
	format.setSampleSize(16);
	format.setBitRate(128);
	format.setCodec(ViCodecManager::selected("AC3"));
	QFile *file = new QFile("/home/visore/test."+format.codec().extensions()[0]);
	mCoder.encode(mBuffer, file, &format);
}

void ViQtFileOutput::stop()
{

}

void ViQtFileOutput::pause()
{

}
