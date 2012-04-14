#include "viqtfileinput.h"

#include "vicodecs.h"
#include <QFile>

ViQtFileInput::ViQtFileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
	: ViFileInput(buffer, metaData, filePath)
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
	ViCodecs::decode(mFilePath, mBuffer);
}

void ViQtFileInput::stop()
{

}

void ViQtFileInput::pause()
{

}
