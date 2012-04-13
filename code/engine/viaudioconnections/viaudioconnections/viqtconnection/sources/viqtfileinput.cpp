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
	QFile file(mFilePath);
if (!file.open(QIODevice::ReadOnly))
         return;

     QByteArray inData = file.readAll();
	QByteArray outData;

cout<<"Codec status: "<<ViCodecs::decode(&inData, &outData, ViCodecs::codecId(mFilePath))<<"  *"<<mFilePath.toAscii().data()<<"*"<<endl;
mStream->write(&outData);

}

void ViQtFileInput::stop()
{

}

void ViQtFileInput::pause()
{

}
