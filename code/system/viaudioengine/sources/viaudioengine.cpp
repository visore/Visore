#include "viaudioengine.h"

#include <QDir>
#include <QCoreApplication>
#include "vifileinput.h"

ViAudioEngine::ViAudioEngine()
	: QObject()
{
	mBuffer = new ViAudioBuffer();
	mAudioInput = NULL;


	/*format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);*/

QAudioFormat format;
WavFile w;
w.open("/home/visore/Desktop/a.wav");
format = w.fileFormat();
ViLogger::debug(QString::number(format.frequency()));
ViLogger::debug(QString::number(format.channels()));
ViLogger::debug(QString::number(format.sampleSize()));
ViLogger::debug(format.codec());
ViLogger::debug(QString::number(format.byteOrder()==QAudioFormat::LittleEndian));
ViLogger::debug(QString::number(format.sampleType()==QAudioFormat::SignedInt));




ViLibrary<ViFileInput> *loader = new ViLibrary<ViFileInput>(QCoreApplication::applicationDirPath()+"/system/connections/libvibassconnection.so");
if(!loader->open()) ViLogger::debug("Library cannot be loaded! ");
ViFileInput *input = loader->create("createFileInput");
input->setBuffer(mBuffer);
input->setFilePath("/home/visore/Desktop/a.mp3");
input->start();

/*ViLibrary<ViFileInput> loader(QCoreApplication::applicationDirPath()+"/system/connections/libvibassconnection.so");
if(!loader.open()) ViLogger::debug("Library cannot be loaded! ");
ViFileInput *input = loader.create("createFileInput"mBuffer, "/home/visore/Desktop/a.wav");*/
////input->start();






















	//Test
	
	/*QAudioFormat format;
	// set up the format you want, eg.
WavFile w;
w.open("/home/visore/Desktop/a.wav");
format = w.fileFormat();

	format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(format)) {
		format = info.nearestFormat(format);
ViLogger::debug("999999999998888888888");
		}
*/
	//initializeInputStream(info, format);
	//initializeInputFile("/home/visore/Desktop/test.raw");
//	initializeOutputStream();
//	mAudioInput->start();
}

ViAudioEngine::~ViAudioEngine()
{
	if(mAudioInput != NULL)
	{
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
		mAudioInput = NULL;
	}
	if(mAudioOutputs.size() != 0)
	{
		for(int i = 0; i < mAudioOutputs.size(); ++i)
		{
			delete mAudioOutputs[i];
			ViObject::disconnect(mAudioOutputs[i], SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
			mAudioOutputs[i] = NULL;
		}
	}
	if(mBuffer != NULL)
	{
		delete mBuffer;
		mBuffer = NULL;
	}
}
/*
void ViAudioEngine::initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format)
{
	if(mAudioInput != NULL)
	{
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
	}
	mInputAudioType = ViAudioEngine::Stream;
	//mAudioInput = new ViStreamInput(mBuffer, deviceInfo, format);
	ViObject::connect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViAudioEngine::initializeInputFile(QString filePath)
{
	if(mAudioInput != NULL)
	{
		ViObject::disconnect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
		delete mAudioInput;
	}
	mInputAudioType = ViAudioEngine::File;
	//mAudioInput = new ViFileInput(mBuffer, filePath);
	ViObject::connect(mAudioInput, SIGNAL(changed(int, int)), this, SLOT(changeReceived(int, int)));
}

void ViAudioEngine::initializeOutputStream()
{
	if(mOutputAudioType == ViAudioEngine::File)
	{
		mOutputAudioType = ViAudioEngine::FileAndStream;
	}
	else if(mOutputAudioType == ViAudioEngine::None)
	{
		mOutputAudioType = ViAudioEngine::Stream;
	}
	//mAudioOutputs.append(new ViStreamOutput(mBuffer));
}

void ViAudioEngine::initializeOutputFile()
{
	if(mOutputAudioType == ViAudioEngine::Stream)
	{
		mOutputAudioType = ViAudioEngine::FileAndStream;
	}
	else if(mOutputAudioType == ViAudioEngine::None)
	{
		mOutputAudioType = ViAudioEngine::File;
	}
	//mAudioOutputs.append(new ViStreamOutput(mBuffer));
}*/

void ViAudioEngine::changeReceived(int startIndex, int size)
{
	//ViLogger::debug("Data received: index "+QString::number(startIndex)+" size "+QString::number(size));
	//ViLogger::debug("yyyyy: "+QString::number(mBuffer->size()));
	//mAudioOutputs[0]->start();
}

