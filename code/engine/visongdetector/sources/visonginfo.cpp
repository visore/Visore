#include "visonginfo.h"

ViSongInfo::ViSongInfo()
{
	mTitle = "";
	mArtist = "";
	mDescription = "";
	mRating = -1;
	mImagePath = "";
}

QString ViSongInfo::title()
{
	return mTitle;
}

QString ViSongInfo::artist()
{
	return mArtist;
}

QString ViSongInfo::description()
{
	return mDescription;
}

qint16 ViSongInfo::rating()
{
	return mRating;
}

QImage ViSongInfo::image()
{
	return QImage(mImagePath);
}

void ViSongInfo::setTitle(QString title)
{
	mTitle = title;
}

void ViSongInfo::setArtist(QString artist)
{
	mArtist = artist;
}

void ViSongInfo::setDescription(QString description)
{
	mDescription = description;
}

void ViSongInfo::setRating(qint16 rating)
{
	mRating = rating;
}

void ViSongInfo::setImagePath(QString path)
{
	mImagePath = path;
}
