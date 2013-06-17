#ifndef VISONGINFO_H
#define VISONGINFO_H

#include <QString>
#include <QList>
#include <QImage>
#include <QByteArray>

class ViSongInfo
{
	public:

		ViSongInfo();
		ViSongInfo(const ViSongInfo &other);

		static QString unknownArtist();
		static QString unknownTitle();

		QString message();
		qint16 score();
		QString description();

		QString songId();
		QString songTitle(bool returnUnknown = false);
		qint16 songHotness();
		qint16 songDanceability();
		qint16 songEnergy();
		double songDuration();
		double songTempo();
		double songLoudness();

		QString artistId();
		QString artistName(bool returnUnknown = false);
		qint16 artistHotness();
		qint16 artistFamiliarity();

		QString imagePath(int index = 0);
		QString imageExtension(int index = 0);
		QString imageExtension(QString prefix, int index = 0);
		QString imageMimeType(int index = 0);
		QImage image(int index = 0);
		QByteArray imageData(int index = 0);

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
		void setImagePath(QString path);
		bool hasImage();

		QString toString();
		void clear();

        bool isValid();

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

typedef QList<ViSongInfo> ViSongInfoList;

#endif
