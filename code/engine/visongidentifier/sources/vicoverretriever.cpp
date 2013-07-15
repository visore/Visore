#include <vicoverretriever.h>
#include <vimanager.h>
#include <vilogger.h>
#include <viid.h>
#include <QFileInfo>
#include <QImageWriter>
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
		emit retrieved(true);
	}
	else if(mUrls.isEmpty())
	{
		emit retrieved(false);
	}
	else
	{
		processNext(mUrls.dequeue());
	}
}

void ViCoverRetriever::processUrls(ViStringQueue urls)
{
	mUrls = urls;
	if(mUrls.isEmpty())
	{
		testImage(false);
	}
	else
	{
		processNext(mUrls.dequeue());
	}
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
	QString path = ViManager::tempCoverPath();
	QDir dir(path);
	if(!dir.exists())
	{
		dir.mkpath(path);
	}
	path += QDir::separator() + ViId().id() + ".png";

	QImageWriter writer(path, "png");
	writer.setQuality(100);
	if(writer.write(QImage::fromData(data)))
	{
		mMetadata.setCover(path);
		return true;
	}
	return false;
}
