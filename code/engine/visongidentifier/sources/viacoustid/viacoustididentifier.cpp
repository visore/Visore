#include <viacoustididentifier.h>
#include <viaudioposition.h>
#include <vipcmconverter.h>
#include <vimanager.h>

#define CHUNK_SIZE 4096

ViAcoustidIdentifierThread::ViAcoustidIdentifierThread()
	: QThread()
{
	mBuffer = NULL;
}

void ViAcoustidIdentifierThread::start(ViBuffer *buffer)
{
	mBuffer = buffer;
	QThread::start();
}

void ViAcoustidIdentifierThread::run()
{
	ViBufferStreamPointer stream = mBuffer->createReadStream();
	ViAudioFormat format = mBuffer->format();
	int seconds = ViAudioPosition::convertPosition(mBuffer->size(), ViAudioPosition::Bytes, ViAudioPosition::Seconds, format);

	ViRawChunk rawChunk(CHUNK_SIZE);

	if(!mCalculator.start(format.sampleRate(), format.channelCount()))
	{
		LOG("The fingerprint calculator could not be initialized.", QtCriticalMsg);
		finished("", 0);
		emit finished("", 0);
		return;
	}

	int size = 0;
	do
	{
		size = stream->read(rawChunk) / 2;
		qint16 *base = reinterpret_cast<qint16*>(rawChunk.data());
		mCalculator.feed(base, size);
	}
	while(size > 0);
	emit finished(mCalculator.finish(), seconds);
}

ViAcoustidIdentifier::ViAcoustidIdentifier()
	: ViSongIdentifier()
{
	QObject::connect(&mThread, SIGNAL(finished(QString, int)), this, SLOT(retieveInfo(QString, int)));
}

void ViAcoustidIdentifier::retieveInfo(QString code, int duration)
{
	if(code == "")
	{
		finish();
	}
	else
	{
		ViWebParameters parameters;
		parameters.append("fingerprint", code);
		parameters.append("format", "json");
		parameters.append("duration", QString::number(duration));
		parameters.append("meta", "recordings+releases");
		parameters.append("client", ViManager::acoustidKey());

		redirectReply(SLOT(processInfo(bool)));
		retrieve("http://api.acoustid.org/v2/lookup", parameters);
	}
}

void ViAcoustidIdentifier::processInfo(bool success)
{
	if(success)
	{
		QVariantMap result = jsonResult().toVariantMap();
		if(result["status"].toString() == "ok")
		{
			QList<QVariant> results = result["results"].toList();
			for(int j = 0; j < results.size(); ++j)
			{
				QList<QVariant> recordings = results[0].toMap()["recordings"].toList();
				if(!recordings.isEmpty())
				{
					ViMetadata metadata;
					QVariantMap infoMap = recordings[0].toMap();
					metadata.setTitle(infoMap["title"].toString());
					QList<QVariant> artistsMap = infoMap["artists"].toList();
					if(!artistsMap.isEmpty())
					{
						metadata.setArtist(artistsMap[0].toMap()["name"].toString());
						finish(metadata);
						return;
					}
				}
			}
		}
	}
	finish();
}

void ViAcoustidIdentifier::identify(ViBuffer *buffer)
{
	reset();
	mThread.start(buffer);
}
