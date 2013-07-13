#include <viacoustididentifier.h>
#include <viaudioposition.h>
#include <vipcmconverter.h>
#include <vimanager.h>
#include <vilogger.h>

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
			QList<QVariant> results = result["results"].toList();
			for(int j = 0; j < results.size(); ++j)
			{
				QList<QVariant> recordings = results[0].toMap()["recordings"].toList();

				// Take the song that has most occurences
				QHash<QString, int> songs;
				QString title;
				for(int i = 0; i < recordings.size(); ++i)
				{
					title = recordings[0].toMap()["title"].toString();
					if(songs.contains(title))
					{
						songs[title] += 1;
					}
					else
					{
						songs[title] = 1;
					}
				}

				QString maxTitle = "";
				int max = 0;
				foreach(title, songs.keys())
				{
					if(songs[title] > max)
					{
						max = songs[title];
						maxTitle = title;
					}
				}

				if(maxTitle != "")
				{
					for(int i = 0; i < recordings.size(); ++i)
					{
						QVariantMap infoMap = recordings[0].toMap();
						title = infoMap["title"].toString();
						if(title == maxTitle)
						{
							ViMetadata metadata;
							metadata.setTitle(title);
							QList<QVariant> artistsMap = infoMap["artists"].toList();
							if(!artistsMap.isEmpty())
							{
								metadata.setArtist(artistsMap[0].toMap()["name"].toString());
								finish(metadata);
								return;
							}
							break;
						}
					}
				}
			}
		}
	}
	finish();
}

void ViAcoustidIdentifier::identify(ViBufferOffsets bufferOffset)
{
	mFingerprinter.generate(bufferOffset);
}
