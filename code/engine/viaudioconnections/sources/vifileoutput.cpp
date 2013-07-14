#include "vifileoutput.h"
#include <QDir>

ViFileOutput::ViFileOutput()
	: ViAudioOutput()
{
	mFilePath = "";
	QObject::connect(&mCoder, SIGNAL(finished()), this, SIGNAL(finished()));
	setState(QAudio::IdleState);
}

void ViFileOutput::setMetadata(ViMetadata metadata)
{
	mMetadata = metadata;
}

void ViFileOutput::clearMetadata()
{
	mMetadata.clear();
}

void ViFileOutput::setFile(QString directory, QString trackNumber, QString extension)
{
	if(!directory.endsWith(QDir::separator()))
	{
		directory.append(QDir::separator());
	}
	if(trackNumber.endsWith(". "))
	{
		directory += trackNumber;
	}
	else if(trackNumber.endsWith("."))
	{
		directory += trackNumber + " ";
	}
	else
	{
		directory += trackNumber + ". ";
	}

	directory += mMetadata.artist() + " - ";
	directory += mMetadata.title();

	if(!extension.startsWith("."))
	{
		directory += ".";
	}
	directory += extension;
	setFile(directory);
}

void ViFileOutput::setFile(QString filePath)
{
	mFilePath = filePath;
}

ViAudioFormat ViFileOutput::format()
{
	return mFormat;
}

void ViFileOutput::setFormat(ViAudioFormat format)
{
	mFormat = format;
}

ViMetadata ViFileOutput::metadata()
{
	return mMetadata;
}

QString ViFileOutput::filePath()
{
	return mFilePath;
}

void ViFileOutput::start()
{
	LOG("Writing to file started.");
	mCoder.encode(mBuffer, mFilePath, mFormat, mMetadata);
	setState(QAudio::ActiveState);
}

void ViFileOutput::stop()
{
	LOG("Writing to file stopped.");
	//mCoder.stop();
	setState(QAudio::StoppedState);
}

void ViFileOutput::pause()
{
	LOG("Writing to file pasued.");
	setState(QAudio::SuspendedState);
}
