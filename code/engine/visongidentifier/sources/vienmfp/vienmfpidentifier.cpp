#include <vienmfpidentifier.h>
#include <vimanager.h>

ViEnmfpIdentifier::ViEnmfpIdentifier()
	: ViSongIdentifier()
{
	setKey(ViManager::enmfpKey());
	QObject::connect(&mFingerprinter, SIGNAL(generated()), this, SLOT(sendRequest()));
}

void ViEnmfpIdentifier::identifyTrack(ViBufferOffsets bufferOffset)
{
	mFingerprinter.generate(bufferOffset);
}

void ViEnmfpIdentifier::sendRequest()
{
	QJsonObject object;
	object.insert("code", QJsonValue(mFingerprinter.fingerprint()));

	redirectReply(SLOT(processReply(bool)));
	//retrieve("http://developer.echonest.com/api/v4/song/identify?api_key=" + key() + "&version=" + mFingerprinter.version() + "&bucket=id:7digital-US&bucket=tracks&bucket=audio_summary&bucket=artist_familiarity&bucket=artist_hotttnesss&bucket=song_hotttnesss", object);
	retrieve("http://developer.echonest.com/api/v4/song/identify?api_key=" + key() + "&version=" + mFingerprinter.version() + "&bucket=id:7digital-US&bucket=tracks", object);
}

void ViEnmfpIdentifier::processReply(bool success)
{
	if(success)
	{
		QVariantMap result = jsonResult().toVariantMap()["response"].toMap();
		QString message = result["status"].toMap()["message"].toString();
		if(message.contains("api") && message.contains("key"))
		{
			finish();
			return;
		}
		QList<ViMetadata> metadatas;
		QVariantList songs = result["songs"].toList();
		for(int i = 0; i < songs.size(); ++i)
		{
			QVariantMap song = songs[i].toMap();
			if(song["message"].toString().toLower().trimmed().startsWith("ok"))
			{
				ViMetadata metadata;
				metadata.setTitle(song["title"].toString());
				metadata.setArtist(song["artist_name"].toString());
				metadatas.append(metadata);
			}
		}
		finish(metadatas);
	}
	else
	{
		finish();
	}
}
