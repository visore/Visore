#include "viqtfileoutput.h"

ViQtFileOutput::ViQtFileOutput()
	: ViFileOutput()
{
}

ViQtFileOutput::~ViQtFileOutput()
{
}

void ViQtFileOutput::start()
{
	mCoder.encode(mBuffer, mFilePath, mBuffer->format(), mFormat);
	emit started();
}

void ViQtFileOutput::stop()
{
	mCoder.stop();
	emit stopped();
}

void ViQtFileOutput::pause()
{
	emit paused();
}
