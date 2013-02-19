#include <vicoverretriever.h>
#include <vimanager.h>
#include <vilogger.h>
#include <viid.h>
#include <QBuffer>
#include <QFileInfo>
#include <QImageReader>
#include <QDir>

ViCoverRetriever::ViCoverRetriever()
	: QObject()
{
	QObject::connect(&mImageServicer, SIGNAL(finished(bool)), this, SLOT(testImage(bool)));
}

ViCoverRetriever::~ViCoverRetriever()
{
}

void ViCoverRetriever::testImage(bool success)
{
	if(success && saveImage(mImageServicer.byteResult()))
	{
		mUrls.clear();
		LOG("Cover image found for \"" + mMetadata.artist() + " - " + mMetadata.title() + "\".");
		emit retrieved(true);
	}
	else if(mUrls.isEmpty())
	{
		LOG("No cover image could be found for \"" + mMetadata.artist() + " - " + mMetadata.title() + "\".");
		emit retrieved(false);
	}
	else
	{
		processNext(mUrls.dequeue());
	}
}

void ViCoverRetriever::processUrls(QStringQueue urls)
{
	mUrls = urls;
	processNext(mUrls.dequeue());
}

void ViCoverRetriever::processNext(QString url)
{
	mImageServicer.retrieve(url);
}

void ViCoverRetriever::retrieve(ViMetadata metadata)
{
	mMetadata = metadata;
	mUrls.clear();
	retrieveUrls();
}

ViMetadata ViCoverRetriever::metadata()
{
	return mMetadata;
}

bool ViCoverRetriever::saveImage(QByteArray &data)
{
	QBuffer buffer(&data);
	buffer.open(QIODevice::ReadOnly);
	QString extension = QString(QImageReader::imageFormat(&buffer)).toLower();
	buffer.close();
	if(extension == "")
	{
		return false;
	}

	QString id = ViId().id();
	QString newPath = ViManager::tempPath() + QDir::separator() + "albumart";
	QDir dir(newPath);
	if(!dir.exists())
	{
		dir.mkpath(newPath);
	}
	newPath += QDir::separator() + id + "." + extension;

	QFileInfo info(newPath);
	QFile file(newPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	file.write(data);
	file.close();

	mMetadata.setCover(newPath);
	return true;
}
