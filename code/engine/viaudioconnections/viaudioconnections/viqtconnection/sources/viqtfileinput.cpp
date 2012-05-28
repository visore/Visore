#include "viqtfileinput.h"

#include "vicoder.h"
#include <QFile>

ViQtFileInput::ViQtFileInput()
	: ViFileInput()
{
}

void ViQtFileInput::start()
{
	mCoder.decode(mFilePath, mBuffer, &mFormat);
	mBuffer->setFormat(mFormat);
	emit formatChanged(mFormat);
	emit started();
}

void ViQtFileInput::stop()
{
	mCoder.stop();
	emit stopped();
}

void ViQtFileInput::pause()
{
	emit paused();
}
