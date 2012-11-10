#include "viechoresponse.h"
#include <QJsonDocument>
#include <QJsonObject>

ViEchoResponse::ViEchoResponse()
{
	reset();
}

void ViEchoResponse::reset()
{
	mVersion = "";
	mCode = "";
	mMessage = "";
	mCurrentSong = -1;
	mSongs.clear();
}

void ViEchoResponse::analyze(QByteArray json)
{
	QJsonDocument document = QJsonDocument::fromJson(json);
	QVariantMap result = document.object().toVariantMap();
	QVariantMap response = result["response"].toMap();
	if(response["images"].toList().size() > 0)
	{
		analyzeImage(response, mCurrentSong);
	}
	else
	{
		analyzeInfo(response);
	}
}

void ViEchoResponse::analyzeInfo(QVariantMap map)
{
	QVariantMap status = map["status"].toMap();
	mVersion = status["version"].toString();
	mCode = status["code"].toString();
	mMessage = status["message"].toString();
	QVariantList songs = map["songs"].toList();
	for(int i = 0; i < songs.size(); ++i)
	{
		QVariantMap song = songs[i].toMap();
		ViSongInfo info;

		if(song["message"].toString().toLower().trimmed().startsWith("ok"))
		{
			info.setScore(song["score"].toInt());
			info.setMessage(song["message"].toString());
			info.setSongId(song["id"].toString());
			info.setSongTitle(song["title"].toString());
			info.setArtistId(song["artist_id"].toString());
			info.setArtistName(song["artist_name"].toString());

			info.setArtistHotness(song["artist_hotttnesss"].toDouble() * 100);
			info.setSongHotness(song["song_hotttnesss"].toDouble() * 100);
			info.setArtistFamiliarity(song["artist_familiarity"].toDouble() * 100);
			QVariantMap summary = song["audio_summary"].toMap();
			info.setSongDuration(summary["duration"].toDouble());
			info.setSongLoudness(summary["loudness"].toDouble());
			info.setSongEnergy(summary["energy"].toDouble() * 100);
			info.setSongTempo(summary["tempo"].toDouble());
			info.setSongDanceability(summary["danceability"].toDouble() * 100);
			QVariantList tracks = song["tracks"].toList();
			for(int j = 0; j < tracks.size(); ++j)
			{
				QVariantMap track = tracks[j].toMap();
				info.addImagePath(track["release_image"].toString());
			}

			mSongs.append(info);
		}
	}
	if(mSongs.size() > 0)
	{
		mCurrentSong = 0;
	}
}

void ViEchoResponse::analyzeImage(QVariantMap map, int songIndex)
{
	QVariantList images = map["images"].toList();
	for(int i = 0; i < images.size(); ++i)
	{
		QVariantMap image = images[i].toMap();
		mSongs[songIndex].addImagePath(image["url"].toString());
	}
}

QString ViEchoResponse::version()
{
	return mVersion;
}

QString ViEchoResponse::code()
{
	return mCode;
}

QString ViEchoResponse::message()
{
	return mMessage;
}

int ViEchoResponse::numberOfSongs()
{
	return mSongs.size();
}

ViSongInfo& ViEchoResponse::songInfo(int index)
{
	if(index < 0)
	{
		return mSongs[mCurrentSong];
	}
	return mSongs[index];
}

QList<ViSongInfo> ViEchoResponse::songInfos()
{
	return mSongs;
}

QString ViEchoResponse::toString()
{
	QString result = "";
	result += "Version: " + mVersion;
	result += "\nCode: " + mCode;
	result += "\nMessage: " + mMessage;
	for(int i = 0; i < mSongs.size(); ++i)
	{
		result += "\n" + mSongs[i].toString();
	}
	return result;
}

QString ViEchoResponse::toUrl(QString string)
{
	return string.replace(" ", "%20");
}