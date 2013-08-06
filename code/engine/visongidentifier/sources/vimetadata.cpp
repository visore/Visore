#include <vimetadata.h>
#include <QImageReader>
#include <QFileInfo>
#include <QBuffer>
#include <QDir>

ViMetadata::ViMetadata()
{
	clear();
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

void ViMetadata::clear()
{
	mArtist = "";
	mTitle = "";
	mAlbum = "";
	mCover = "";
}

bool ViMetadata::isValid() const
{
	return hasArtist() || hasTitle() || hasAlbum() || hasCover();
}

void ViMetadata::setArtist(const QString &artist)
{
	if(QString(artist).replace(" ", "").toLower() == unknownArtist().replace(" ", "").toLower())
	{
		mArtist = "";
	}
	else
	{
		mArtist = artist;
	}
}

void ViMetadata::setTitle(const QString &title)
{
	if(QString(title).replace(" ", "").toLower() == unknownTitle().replace(" ", "").toLower())
	{
		mTitle = "";
	}
	else
	{
		mTitle = title;
	}
}

void ViMetadata::setAlbum(const QString &album)
{
	if(QString(album).replace(" ", "").toLower() == unknownAlbum().replace(" ", "").toLower())
	{
		mAlbum = "";
	}
	else
	{
		mAlbum = album;
	}
}

void ViMetadata::setCover(const QString &cover)
{
	mCover = cover;
}

QString ViMetadata::artist(const bool &returnDefault) const
{
	if(hasArtist())
	{
		return mArtist;
	}
	else if(returnDefault)
	{
		return unknownArtist();
	}
	return "";
}

QString ViMetadata::title(const bool &returnDefault) const
{
	if(hasTitle())
	{
		return mTitle;
	}
	else if(returnDefault)
	{
		return unknownTitle();
	}
	return "";
}

QString ViMetadata::album(const bool &returnDefault) const
{
	if(hasAlbum())
	{
		return mAlbum;
	}
	else if(returnDefault)
	{
		return unknownAlbum();
	}
	return "";
}

QString ViMetadata::cover() const
{
	return mCover;
}

bool ViMetadata::hasArtist() const
{
	return mArtist != "";
}

bool ViMetadata::hasTitle() const
{
	return mTitle != "";
}

bool ViMetadata::hasAlbum() const
{
	return mAlbum != "";
}

bool ViMetadata::hasCover() const
{
	return mCover != "";
}

QImage ViMetadata::coverImage() const
{
	return QImage(mCover);
}

QByteArray ViMetadata::coverData() const
{
	if(hasCover())
	{
		QByteArray data;
		QBuffer buffer(&data);
		buffer.open(QIODevice::WriteOnly);
		coverImage().save(&buffer, coverFormat().toLatin1().data());
		return data;
	}
	return QByteArray();
}

QString ViMetadata::coverFormat() const
{
	return QString(QImageReader::imageFormat(mCover)).toLower();
}

QString ViMetadata::coverMimeType() const
{
	if(hasCover())
	{
		return "image/" + coverFormat();
	}
	return "";
}

bool ViMetadata::copyCover(const QString &newPath, const bool &deleteOld)
{
	if(hasCover())
	{
		QString path = newPath;
		if(QFileInfo(path).isDir())
		{
			if(!path.endsWith(QDir::separator()))
			{
				path += QDir::separator();
			}
			path += QFileInfo(mCover).fileName();
		}
		if(QFile::copy(mCover, path))
		{
			if(deleteOld) QFile::remove(mCover);
			setCover(path);
			return true;
		}
	}
	return false;
}

QString ViMetadata::toString() const
{
	QString result = "Metadata:";
	result += "\n\tTitle: " + title();
	result += "\n\tArtist: " + artist();
	result += "\n\tAlbum: " + album();
	result += "\n\tCover: " + cover();
	return result;
}

QString ViMetadata::toShortString() const
{
	return artist() + " - " + title();
}

QString ViMetadata::unknownArtist()
{
	return "Unknown Artist";
}

QString ViMetadata::unknownTitle()
{
	return "Unknown Title";
}

QString ViMetadata::unknownAlbum()
{
	return "Unknown Album";
}

bool ViMetadata::operator == (const ViMetadata &other) const
{
	return title() == other.title() && artist() == other.artist() && album() == other.album();
}

bool ViMetadata::operator != (const ViMetadata &other) const
{
	return !(operator == (other));
}
