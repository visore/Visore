#include "visonginfo.h"

ViSongInfo::ViSongInfo()
{
	mMessage = "";
	mScore = -1;
	mDescription = "";

	mSongId = "";
	mSongTitle = "";
	mSongHotness = -1;
	mSongDanceability = -1;
	mSongEnergy = -1;
	mSongDuration = -1;
	mSongTempo = -1;
	mSongLoudness = -1;
	
	mArtistId = "";
	mArtistName = "";
	mArtistHotness = -1;
	mArtistFamiliarity = -1;
	
	mImagePath = "";
}

QString ViSongInfo::message()
{
	return mMessage;
}

qint16 ViSongInfo::score()
{
	return mScore;
}

QString ViSongInfo::description()
{
	return mDescription;
}

QString ViSongInfo::songId()
{
	return mSongId;
}

QString ViSongInfo::songTitle()
{
	return mSongTitle;
}

qint16 ViSongInfo::songHotness()
{
	return mSongHotness;
}

qint16 ViSongInfo::songDanceability()
{
	return mSongDanceability;
}

qint16 ViSongInfo::songEnergy()
{
	return mSongEnergy;
}

double ViSongInfo::songDuration()
{
	return mSongDuration;
}

double ViSongInfo::songTempo()
{
	return mSongTempo;
}

double ViSongInfo::songLoudness()
{
	return mSongLoudness;
}

QString ViSongInfo::artistId()
{
	return mArtistId;
}

QString ViSongInfo::artistName()
{
	return mArtistName;
}

qint16 ViSongInfo::artistHotness()
{
	return mArtistHotness;
}

qint16 ViSongInfo::artistFamiliarity()
{
	return mArtistFamiliarity;
}

QImage ViSongInfo::image()
{
	return QImage(mImagePath);
}

void ViSongInfo::setMessage(QString message)
{
	mMessage = message;
}

void ViSongInfo::setScore(qint16 score)
{
	mScore = score;
}

void ViSongInfo::setDescription(QString description)
{
	mDescription = description;
}

void ViSongInfo::setSongId(QString id)
{
	mSongId = id;
}

void ViSongInfo::setSongTitle(QString title)
{
	mSongTitle = title;
}

void ViSongInfo::setSongHotness(qint16 hotness)
{
	mSongHotness = hotness;
}

void ViSongInfo::setSongDanceability(qint16 danceability)
{
	mSongDanceability = danceability;
}

void ViSongInfo::setSongEnergy(qint16 energy)
{
	mSongEnergy = energy;
}

void ViSongInfo::setSongDuration(double duration)
{
	mSongDuration = duration;
}

void ViSongInfo::setSongTempo(double tempo)
{
	mSongTempo = tempo;
}

void ViSongInfo::setSongLoudness(double loudness)
{
	mSongLoudness = loudness;
}

void ViSongInfo::setArtistId(QString id)
{
	mArtistId = id;
}

void ViSongInfo::setArtistName(QString name)
{
	mArtistName = name;
}

void ViSongInfo::setArtistHotness(qint16 hotness)
{
	mArtistHotness = hotness;
}

void ViSongInfo::setArtistFamiliarity(qint16 familiarity)
{
	mArtistFamiliarity = familiarity;
}

void ViSongInfo::setImagePath(QString path)
{
	mImagePath = path;
}

QString ViSongInfo::toString()
{
	QString result = "Song:";
	result += "\n\tScore: " + QString::number(mScore);
	result += "\n\tMessage: " + mMessage;
	result += "\n\tDescription: " + mDescription;
	result += "\n\tSong ID: " + mSongId;
	result += "\n\tSong Title: " + mSongTitle;
	result += "\n\tSong Hotness: " + QString::number(mSongHotness);
	result += "\n\tSong Danceability: " + QString::number(mSongDanceability);
	result += "\n\tSong Energy: " + QString::number(mSongEnergy);
	result += "\n\tSong Duration: " + QString::number(mSongDuration);
	result += "\n\tSong Tempo: " + QString::number(mSongTempo);
	result += "\n\tArtist ID: " + mArtistId;
	result += "\n\tArtist Name: " + mArtistName;
	result += "\n\tArtist Hotness: " + QString::number(mArtistHotness);
	result += "\n\tArtist Familiarity: " + QString::number(mArtistFamiliarity);
	result += "\n\tImage: " + mImagePath;
	return result;
}
