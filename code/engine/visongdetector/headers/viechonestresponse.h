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
		void analyze(QByteArray json);
		QString version();
		QString code();
		QString message();
		int numberOfSongs();
		ViSongInfo songInfo(int index = -1);
		bool hasError();
		QString toString();
		static QString toUrl(QString string);

	private:
		QList<ViSongInfo> mSongs;
		bool mHasError;
		QString mVersion;
		QString mCode;
		QString mMessage;
		qint16 mCurrentSong;
};

#endif
