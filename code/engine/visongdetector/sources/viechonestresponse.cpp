#include "viechonestresponse.h"
#include <QJsonDocument>

ViEchoNestResponse::ViEchoNestResponse()
{
	mVersion = "";
	mCode = "";
	mMessage = "";
	mCurrentSong = -1;
	mHasError = false;
}

void ViEchoNestResponse::analyze(QByteArray json)
{
	QJsonDocument document = QJsonDocument::fromJson(json);
	bool ok;
	//QVariantMap result = parser.toJson();
	/*if(!ok)
	{
		mHasError = true;
	}
	QVariantMap response = result["response"].toMap();
	QVariantMap status = response["status"].toMap();
	mVersion = status["version"].toString();
	mCode = status["code"].toString();
	mMessage = status["message"].toString();
	QVariantList songs = response["songs"].toList();
	for(int i = 0; i < songs.size(); ++i)
	{
		QVariantMap song = songs[i].toMap();
		ViSongInfo info;

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
		if(tracks.size() > 0)
		{
			QVariantMap track = tracks[0].toMap();
			info.setImagePath(track["release_image"].toString());
		}

		mSongs.append(info);
	}
	if(mSongs.size() > 0)
	{
		mCurrentSong = 0;
	}*/
}

QString ViEchoNestResponse::version()
{
	return mVersion;
}

QString ViEchoNestResponse::code()
{
	return mCode;
}

QString ViEchoNestResponse::message()
{
	return mMessage;
}

int ViEchoNestResponse::numberOfSongs()
{
	return mSongs.size();
}

ViSongInfo ViEchoNestResponse::songInfo(int index)
{
	if(index < 0)
	{
		return mSongs[mCurrentSong];
	}
	return mSongs[index];
}

bool ViEchoNestResponse::hasError()
{
	return mHasError;
}

QString ViEchoNestResponse::toString()
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

QString ViEchoNestResponse::toUrl(QString string)
{
	return string.replace(" ", "%20");
}
