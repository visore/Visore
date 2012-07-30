#include "vifilesonginfo.h"

ViFileSongInfo::ViFileSongInfo()
	 : ViSongInfo()
{
	mOriginalFilePath = "";
	mCorrectedFilePath = "";
}

ViFileSongInfo::ViFileSongInfo(const ViSongInfo &other)
	 : ViSongInfo(other)
{
	mOriginalFilePath = "";
	mCorrectedFilePath = "";
}

ViFileSongInfo::ViFileSongInfo(const ViFileSongInfo &other)
	 : ViSongInfo(other)
{
	mOriginalFilePath = other.mOriginalFilePath;
	mCorrectedFilePath = other.mCorrectedFilePath;
}

QString ViFileSongInfo::originalFilePath()
{
	return mOriginalFilePath;
}

QString ViFileSongInfo::correctedFilePath()
{
	return mCorrectedFilePath;
}

QString ViFileSongInfo::albumArtPath()
{
	return imagePath();
}

QImage ViFileSongInfo::albumArtImage()
{
	return QImage(albumArtPath());
}

void ViFileSongInfo::setOriginalFilePath(QString path)
{
	mOriginalFilePath = path;
}

void ViFileSongInfo::setCorrectedFilePath(QString path)
{
	mCorrectedFilePath = path;
}

void ViFileSongInfo::setAlbumArtPath(QString path)
{
	setImagePath(path);
}
