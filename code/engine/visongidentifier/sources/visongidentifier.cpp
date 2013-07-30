#include <visongidentifier.h>
#include <vilogger.h>
#include <vimanager.h>
#include <QDir>
#include <QImageReader>

ViSongIdentifier::ViSongIdentifier()
{
	mFound = false;
	mKey = "";
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, SLOT(processReply(bool)));
}

ViSongIdentifier::~ViSongIdentifier()
{
}

ViMetadata ViSongIdentifier::metadata(QList<ViSongIdentifier*> identifiers)
{
	QList<ViMetadata> metadatas;
	for(int i = 0; i < identifiers.size(); ++i)
	{
		metadatas.append(identifiers[i]->metadatas());
	}
	return metadata(metadatas);
}

ViMetadata ViSongIdentifier::metadata(QList<ViMetadata> metadatas)
{
	QList<QPair<ViMetadata, int>> ranks;
	bool found;
	for(int i = 0; i < metadatas.size(); ++i)
	{
		found = false;
		for(int j = 0; j < ranks.size(); ++j)
		{
			if(ranks[j].first == metadatas[i])
			{
				found = true;
				ranks[j].second += 1;
			}
		}
		if(!found)
		{
			ranks.append(QPair<ViMetadata, int>(metadatas[i], 1));
		}
	}

	ViMetadata topMetadata;
	int topCount = 0, count;
	for(int i = 0; i < ranks.size(); ++i)
	{
		count = ranks[i].second;
		if(count > topCount)
		{
			topMetadata = ranks[i].first;
			topCount = count;
		}
	}
	return topMetadata;
}

bool ViSongIdentifier::found()
{
	return mFound;
}

QList<ViMetadata> ViSongIdentifier::metadatas()
{
	return mMetadatas;
}

void ViSongIdentifier::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	mServicer.setProxy(type, host, port, username, password);
}

QNetworkReply::NetworkError ViSongIdentifier::networkError()
{
	return mServicer.error();
}

QString ViSongIdentifier::errorString()
{
	return mServicer.errorString();
}

void ViSongIdentifier::setKey(QString key)
{
	mKey = key;
}

QString ViSongIdentifier::key()
{
	return mKey;
}

void ViSongIdentifier::identify(ViBufferOffsets bufferOffset)
{
	reset();
	identifyTrack(bufferOffset);
}

void ViSongIdentifier::reset()
{
	mMetadatas.clear();
	mFound = false;
	mServicer.disconnect();
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, SLOT(processReply(bool)));
}

void ViSongIdentifier::finish()
{
	mFound = false;
	emit identified(mFound);
}

void ViSongIdentifier::finish(QList<ViMetadata> metadatas)
{
	mMetadatas = metadatas;
	mFound = !mMetadatas.isEmpty();
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
