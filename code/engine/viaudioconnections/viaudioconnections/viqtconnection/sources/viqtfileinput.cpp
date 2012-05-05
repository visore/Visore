#include "viqtfileinput.h"

#include "vicoder.h"
#include <QFile>

ViQtFileInput::ViQtFileInput()
	: ViFileInput()
{
}

ViQtFileInput::~ViQtFileInput()
{
	free();
}

void ViQtFileInput::initialize()
{
	ViFileInput::initialize();
}

void ViQtFileInput::free()
{
	ViFileInput::free();
}

void ViQtFileInput::start()
{
	mBuffer->setFormat(mFormat);
	mCoder.decode(mFilePath, mBuffer, &mFormat);
	emit formatChanged(mFormat);
}

void ViQtFileInput::stop()
{
	mCoder.stop();
}

void ViQtFileInput::pause()
{
}
