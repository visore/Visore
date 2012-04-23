#include "viqtfileinput.h"

#include "vicoder.h"
#include <QFile>

ViQtFileInput::ViQtFileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath)
	: ViFileInput(format, buffer, filePath)
{

}

ViQtFileInput::~ViQtFileInput()
{

}

void ViQtFileInput::initialize()
{

}

void ViQtFileInput::free()
{

}

void ViQtFileInput::start()
{
	cout << mFormat.sampleRate() << " "<< mFormat.sampleSize()<<endl;
	mCoder.decode(mFilePath, mBuffer, &mFormat);
	cout << mFormat.sampleRate() << " "<< mFormat.codec().name().toAscii().data()<<endl;
}

void ViQtFileInput::stop()
{

}

void ViQtFileInput::pause()
{

}
