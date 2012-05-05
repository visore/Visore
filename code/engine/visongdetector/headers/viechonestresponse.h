#ifndef VIECHONESTRESPONSE_H
#define VIECHONESTRESPONSE_H

#include <QString>
#include <QList>
#include <QVariantMap>
#include "visonginfo.h"

class ViEchoNestResponse
{
	public:
		ViEchoNestResponse();
		void reset();
		void analyze(QByteArray json);
		QString version();
		QString code();
		QString message();
		int numberOfSongs();
		ViSongInfo songInfo(int index = -1);
		QList<ViSongInfo> songInfos();
		QString toString();
		static QString toUrl(QString string);

	private:
		void analyzeInfo(QVariantMap map);
		void analyzeImage(QVariantMap map, int songIndex);

	private:
		QList<ViSongInfo> mSongs;
		QString mVersion;
		QString mCode;
		QString mMessage;
		qint16 mCurrentSong;
};

#endif
