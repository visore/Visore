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
	mCoder.encode(mBuffer, mFilePath, &mFormat);
}

void ViQtFileOutput::stop()
{
	mCoder.stop();
}

void ViQtFileOutput::pause()
{

}
