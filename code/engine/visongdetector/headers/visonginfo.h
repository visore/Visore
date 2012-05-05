#ifndef VISONGINFO_H
#define VISONGINFO_H

#include <QString>
#include <QList>

class ViSongInfo
{
	public:
		ViSongInfo();

		QString message();
		qint16 score();
		QString description();

		QString songId();
		QString songTitle();
		qint16 songHotness();
		qint16 songDanceability();
		qint16 songEnergy();
		double songDuration();
		double songTempo();
		double songLoudness();

		QString artistId();
		QString artistName();
		qint16 artistHotness();
		qint16 artistFamiliarity();

		QString imagePath(int index = 0);

		void setMessage(QString message);
		void setScore(qint16 score);
		void setDescription(QString description);

		void setSongId(QString id);
		void setSongTitle(QString title);
		void setSongHotness(qint16 hotness);
		void setSongDanceability(qint16 danceability);
		void setSongEnergy(qint16 energy);
		void setSongDuration(double duration);
		void setSongTempo(double tempo);
		void setSongLoudness(double loudness);

		void setArtistId(QString id);
		void setArtistName(QString name);
		void setArtistHotness(qint16 hotness);
		void setArtistFamiliarity(qint16 familiarity);

		void addImagePath(QString path);
		void changeImagePath(QString fromPath, QString toPath);

		QString toString();

	private:
		QString mMessage;
		qint16 mScore;
		QString mSongId;
		QString mSongTitle;
		qint16 mSongHotness;
		qint16 mSongDanceability;
		qint16 mSongEnergy;
		qreal mSongDuration;
		qreal mSongTempo;
		qreal mSongLoudness;
		QString mArtistId;
		QString mArtistName;
		qint16 mArtistHotness;
		qint16 mArtistFamiliarity;
		QString mDescription;
		QList<QString> mImagePaths;
		
};

#endif
