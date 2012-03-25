#ifndef VISONGINFO_H
#define VISONGINFO_H

#include <QString>
#include <QImage>

class ViSongInfo
{
	public:
		ViSongInfo();

		QString title();
		QString artist();
		QString description();
		qint16 rating();
		QImage image();

		void setTitle(QString title);
		void setArtist(QString artist);
		void setDescription(QString description);
		void setRating(qint16 rating);
		void setImagePath(QString path);

	private:
		QString mTitle;
		QString mArtist;
		QString mDescription;
		qint16 mRating;
		QString mImagePath;
		
};

#endif
