#include <visongidentifier.h>
#include <vilogger.h>
#include <vimanager.h>
#include <QDir>
#include <QImageReader>

ViSongIdentifier::ViSongIdentifier()
{
	mFound = false;
	mKey = "";
	mDescription = "";
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, SLOT(processReply(bool)));
}

ViSongIdentifier::~ViSongIdentifier()
{
}

bool ViSongIdentifier::found()
{
	return mFound;
}

ViMetadata ViSongIdentifier::metadata()
{
	return mMetadata;
}

void ViSongIdentifier::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	mServicer.setProxy(type, host, port, username, password);
}

QNetworkReply::NetworkError ViSongIdentifier::networkError()
{
	return mServicer.error();
}

void ViSongIdentifier::setKey(QString key)
{
	mKey = key;
}

QString ViSongIdentifier::key()
{
	return mKey;
}

void ViSongIdentifier::identify(ViBufferOffsets bufferOffset, QString description)
{
	reset();
	mDescription = description;
	identify(bufferOffset);
}

void ViSongIdentifier::reset()
{
	mMetadata = ViMetadata();
	mFound = false;
	mDescription = "";
	mServicer.disconnect();
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, SLOT(processReply(bool)));
}

void ViSongIdentifier::finish()
{
	LOG("The track's metadata could not be detected (" + mDescription + ").");
	mFound = false;
	emit identified(mFound);
}

void ViSongIdentifier::finish(ViMetadata metadata)
{
	LOG("The track's metadata was detected as \"" + metadata.artist() + " - " + metadata.title() + "\" (" + mDescription + ").");
	mMetadata = metadata;
	mFound = true;
	emit identified(mFound);
}

void ViSongIdentifier::retrieve(QString url)
{
	mServicer.retrieve(url);
}

void ViSongIdentifier::retrieve(QString url, QJsonObject jsonObject)
{
	mServicer.retrieve(url, jsonObject);
}

void ViSongIdentifier::retrieve(QString url, ViWebParameters parameters)
{
	mServicer.retrieve(url, parameters);
}

QString ViSongIdentifier::saveImage(QByteArray &data, QString name)
{
	QBuffer buffer(&data);
	buffer.open(QIODevice::ReadOnly);
	QString extension = QString(QImageReader::imageFormat(&buffer)).toLower();
	buffer.close();

	QString newPath = ViManager::tempPath() + QDir::separator() + "albumart";
	QDir dir(newPath);
	if(!dir.exists())
	{
		dir.mkpath(newPath);
	}
	newPath += QDir::separator() + name + "." + extension;

	QFileInfo metadata(newPath);
	QFile file(newPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return "";
	}
	file.write(data);
	file.close();
	return newPath;
}

void ViSongIdentifier::processReply(bool success)
{
}

void ViSongIdentifier::redirectReply(const char *slot)
{
	mServicer.disconnect();
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, slot);
}

QString ViSongIdentifier::stringResult()
{
	return mServicer.stringResult();
}

QJsonObject ViSongIdentifier::jsonResult()
{
	return mServicer.jsonResult();
}

QByteArray& ViSongIdentifier::byteResult()
{
	return mServicer.byteResult();
}

QString ViSongIdentifier::url()
{
	return mServicer.url();
}
