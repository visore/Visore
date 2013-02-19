#include <vimetadata.h>

ViMetadata::ViMetadata()
{
	mArtist = "";
	mTitle = "";
	mAlbum = "";
	mCover = "";
}

ViMetadata::ViMetadata(const ViMetadata &other)
{
	mArtist = other.mArtist;
	mTitle = other.mTitle;
	mAlbum = other.mAlbum;
	mCover = other.mCover;
}

ViMetadata::~ViMetadata()
{
}

void ViMetadata::setArtist(QString artist)
{
	mArtist = artist;
}

void ViMetadata::setTitle(QString title)
{
	mTitle = title;
}

void ViMetadata::setAlbum(QString album)
{
	mAlbum = album;
}

void ViMetadata::setCover(QString cover)
{
	mCover = cover;
}

QString ViMetadata::artist()
{
	return mArtist;
}

QString ViMetadata::title()
{
	return mTitle;
}

QString ViMetadata::album()
{
	return mAlbum;
}

QString ViMetadata::cover()
{
	return mCover;
}

QImage ViMetadata::coverImage()
{
	return QImage(mCover);
}
