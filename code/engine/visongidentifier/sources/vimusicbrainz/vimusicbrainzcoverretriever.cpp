#include <vimusicbrainzcoverretriever.h>
#include <QPair>
#include <QJsonDocument>
#include <QJsonObject>

#define MAXIMUM_YEAR 3000

ViMusicBrainzCoverRetriever::ViMusicBrainzCoverRetriever()
	: ViCoverRetriever()
{
	QObject::connect(&mServicer, SIGNAL(finished(bool)), this, SLOT(processReply(bool)));
}

void ViMusicBrainzCoverRetriever::processReply(bool success)
{
	if(success)
	{
		QHash<int, QPair<QDate, QString> > unsortedUrls;
		QVariantList recordings = QJsonDocument::fromJson(mServicer.byteResult()).object().toVariantMap().value("recording").toList();
		foreach(QVariant recording, recordings)
		{
			QVariantMap recordingMap = recording.toMap();
			int priority = 0;
			
			//Title
			QString resultTitle = recordingMap.value("title").toString().toLower().trimmed();
			QString dataTitle = mMetadata.title().toLower().trimmed();
			bool hasTitle = true;
			if(resultTitle == dataTitle)
			{
				priority += 2;
			}
			else if(resultTitle.contains(dataTitle) || dataTitle.contains(resultTitle))
			{
				priority += 1;
			}
			else
			{
				hasTitle = false;
			}

			//Artist
			QVariantList possibleArtists = recordingMap.value("artist-credit").toList();
			bool hasArtist = true;
			foreach(QVariant possibleArtist, possibleArtists)
			{
				QVariantMap possibleArtistMap = possibleArtist.toMap();
				QString resultArtist = possibleArtistMap.value("artist").toMap().value("name").toString().toLower().trimmed();
				QString dataArtist = mMetadata.artist().toLower().trimmed();
				if(resultArtist == dataArtist)
				{
					priority += 4;
					break;
				}
				else if(resultArtist.contains(dataArtist) || dataArtist.contains(resultArtist))
				{
					priority += 3;
					break;
				}
				else
				{
					hasArtist = false;
				}
			}

			QVariantList releases = recordingMap.value("releases").toList();
			int totalPriority;
			foreach(QVariant release, releases)
			{
				QVariantMap releaseMap = release.toMap();
				totalPriority = priority;
				if(hasTitle && hasArtist && mMetadata.album() != "")
				{
					QString resultAlbum = releaseMap.value("title").toString().toLower().trimmed();
					QString dataAlbum = mMetadata.album().toLower().trimmed();
					if(resultAlbum == dataAlbum)
					{
						totalPriority += 6;
					}
					else if(resultAlbum.contains(dataAlbum) || dataAlbum.contains(resultAlbum))
					{
						totalPriority += 5;
					}
					else
					{
						totalPriority += 1;
					}
				}
				unsortedUrls.insertMulti(totalPriority, QPair<QDate, QString>(getDate(releaseMap.value("date").toString()), "http://coverartarchive.org/release/" + releaseMap.value("id").toString() + "/front"));
			}
		}

		ViStringQueue sortedUrls;
		while(!unsortedUrls.isEmpty())
		{
			int maximum = -1;
			foreach(int key, unsortedUrls.keys())
			{
				if(key > maximum)
				{
					maximum = key;
				}
			}
			QList<QPair<QDate, QString> > values = unsortedUrls.values(maximum);
			while(!values.isEmpty())
			{			
				QDate firstDate(MAXIMUM_YEAR, 1, 1);
				int index = -1;
				for(int i = 0; i < values.size(); ++i)
				{
					if(values[i].first < firstDate)
					{
						firstDate = values[i].first;
						index = i;
					}
				}
				sortedUrls.enqueue(values.takeAt(index).second);
			}
			unsortedUrls.remove(maximum);
		}

		processUrls(sortedUrls);
	}
	else
	{
		processUrls(ViStringQueue());
	}
}

void ViMusicBrainzCoverRetriever::retrieveUrls()
{
	QString url = "http://www.musicbrainz.org/ws/2/recording?fmt=json&query=%22" + mMetadata.title().replace(" ", "%20") + "%22%20AND%20artist:" + mMetadata.artist().replace(" ", "%20");
	mServicer.retrieve(url);
}

QDate ViMusicBrainzCoverRetriever::getDate(QString string)
{
	if(string == "")
	{
		return QDate(MAXIMUM_YEAR - 1, 1, 1);
	}
	QDate result = QDate::fromString(string, "yyyy");
	if(result.isValid())
	{
		return result;
	}
	result = QDate::fromString(string, "yyyy-MM");
	if(result.isValid())
	{
		return result;
	}
	result = QDate::fromString(string, "yyyy-MM-dd");
	if(result.isValid())
	{
		return result;
	}
	return QDate(MAXIMUM_YEAR - 1, 1, 1);
}
