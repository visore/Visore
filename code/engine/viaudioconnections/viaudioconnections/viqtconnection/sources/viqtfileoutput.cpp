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
	QFile *file = new QFile("/home/visore/b.mp3");
	ViAudioFormat format;
	format.setChannelCount(2);
	format.setSampleRate(44100);
	format.setSampleSize(16);
	format.setBitRate(128);
	format.setCodec(ViCodecManager::selected("MP3"));
	mCoder.encode(mBuffer, file, &format);
}

void ViQtFileOutput::stop()
{

}

void ViQtFileOutput::pause()
{

}
