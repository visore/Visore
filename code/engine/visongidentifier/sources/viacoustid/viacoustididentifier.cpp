#include <viacoustididentifier.h>
#include <viaudioposition.h>
#include <vipcmconverter.h>
#include <vimanager.h>

// For some reason, adding a couple of seconds (2 - 5) to the request duration, will result in a positive retrieval.
// Not sure why this is, but if you play the encoded FLAC files, the play a couple of seconds longer than the indicated duration. Maybe it has something to do with this?
#define DURATION_OVERHEAD 2 // seconds

ViAcoustidIdentifier::ViAcoustidIdentifier()
	: ViSongIdentifier()
{
	QObject::connect(&mFingerprinter, SIGNAL(generated()), this, SLOT(sendRequest()));
}

void ViAcoustidIdentifier::sendRequest()
{
	QString code = mFingerprinter.fingerprint();
	if(code == "")
	{
		finish();
	}
	else
	{
		ViWebParameters parameters;
		parameters.append("fingerprint", code);
		parameters.append("format", "json");
		parameters.append("duration", QString::number(mFingerprinter.duration().seconds() + DURATION_OVERHEAD, 'f', 0)); //No decimal values allowed
		parameters.append("meta", "recordings");
		parameters.append("client", ViManager::acoustidKey());

		redirectReply(SLOT(processReply(bool)));
		retrieve("http://api.acoustid.org/v2/lookup", parameters);
	}
}

void ViAcoustidIdentifier::processReply(bool success)
{
	if(success)
	{
		QVariantMap result = jsonResult().toVariantMap();
		if(result["status"].toString() == "ok")
		{
			QList<ViMetadata> metadatas;
			QList<QVariant> results = result["results"].toList();
			for(int i = 0; i < results.size(); ++i)
			{
				QList<QVariant> recordings = results[i].toMap()["recordings"].toList();
				for(int j = 0; j < recordings.size(); ++j)
				{
					ViMetadata metadata;
					QVariantMap infoMap = recordings[j].toMap();
					metadata.setTitle(infoMap["title"].toString());
					QList<QVariant> artistsMap = infoMap["artists"].toList();
					if(!artistsMap.isEmpty())
					{
						metadata.setArtist(artistsMap[0].toMap()["name"].toString());
					}
					metadatas.append(metadata);
				}
			}
			finish(metadatas);
			return;
		}
	}
	finish();
}

void ViAcoustidIdentifier::identifyTrack(ViBufferOffsets bufferOffset)
{
	mFingerprinter.generate(bufferOffset);
}
