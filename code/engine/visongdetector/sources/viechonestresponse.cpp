#include "viechonestresponse.h"
#include "parser.h"

ViEchoNestResponse::ViEchoNestResponse()
{
	mVersion = "";
	mCode = "";
	mMessage = "";
	mCurrentSong = -1;
	mHasError = false;
}

void ViEchoNestResponse::analyzeSongIdentify(QByteArray json)
{
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(json, &ok).toMap();
	if(!ok)
	{
		mHasError = true;
	}
	QVariantMap response = result["response"].toMap();
	QVariantMap status = response["status"].toMap();
	mVersion = response["version"].toString();
	mCode = response["code"].toString();
	mMessage = response["message"].toString();
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
		mSongs.append(info);
	}
	if(mSongs.size() > 0)
	{
		mCurrentSong = 0;
	}
}

void ViEchoNestResponse::analyzeSongSearch(QByteArray json)
{
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(json, &ok).toMap();
	if(!ok)
	{
		mHasError = true;
	}
	QVariantMap response = result["response"].toMap();
	QVariantList songs = response["songs"].toList();
	if(songs.size() > 0)
	{
		QVariantMap song = songs[0].toMap();
		mSongs[mCurrentSong].setArtistHotness(song["artist_hotttnesss"].toDouble() * 100);
		mSongs[mCurrentSong].setSongHotness(song["song_hotttnesss"].toDouble() * 100);
		mSongs[mCurrentSong].setArtistFamiliarity(song["artist_familiarity"].toDouble() * 100);
		QVariantMap summary = song["audio_summary"].toMap();
		mSongs[mCurrentSong].setSongDuration(summary["duration"].toDouble());
		mSongs[mCurrentSong].setSongLoudness(summary["loudness"].toDouble());
		mSongs[mCurrentSong].setSongEnergy(summary["energy"].toDouble() * 100);
		mSongs[mCurrentSong].setSongTempo(summary["tempo"].toDouble());
		mSongs[mCurrentSong].setSongDanceability(summary["danceability"].toDouble() * 100);
		QVariantList tracks = song["tracks"].toList();
		if(tracks.size() > 0)
		{
			QVariantMap track = tracks[0].toMap();
			mSongs[mCurrentSong].setImagePath(track["release_image"].toString());
		}
	}
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
