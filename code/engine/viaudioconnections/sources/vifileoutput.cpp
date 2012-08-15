#include "vifileoutput.h"
#include <QDir>

ViFileOutput::ViFileOutput()
	: ViAudioOutput()
{
	mFilePath = "";
	QObject::connect(&mCoder, SIGNAL(finished()), this, SIGNAL(finished()));
	setState(QAudio::IdleState);
}

void ViFileOutput::setSongInfo(ViSongInfo info)
{
	mSongInfo = info;
}

void ViFileOutput::clearSongInfo()
{
	mSongInfo.clear();
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
	if(mSongInfo.artistName() == "")
	{
		directory += "Unknown Artist - ";
	}
	else
	{
		directory += mSongInfo.artistName() + " - ";
	}
	if(mSongInfo.songTitle() == "")
	{
		directory += "Unknown Title";
	}
	else
	{
		directory += mSongInfo.songTitle();
	}
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

ViSongInfo ViFileOutput::songInfo()
{
	return mSongInfo;
}

QString ViFileOutput::filePath()
{
	return mFilePath;
}

void ViFileOutput::start()
{
	LOG("Writing to file started.");
	mCoder.encode(mBuffer, mFilePath, mFormat, mSongInfo);	
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
