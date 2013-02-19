#ifndef VIMETADATA_H
#define VIMETADATA_H

#include <QString>
#include <QImage>

class ViMetadata
{

	public:

		ViMetadata();
		ViMetadata(const ViMetadata &other);
		virtual ~ViMetadata();

		void setArtist(QString artist);
		void setTitle(QString title);
		void setAlbum(QString album);
		void setCover(QString cover);

		QString artist();
		QString title();
		QString album();
		QString cover();

		QImage coverImage();

	private:

		QString mArtist;
		QString mTitle;
		QString mAlbum;
		QString mCover;

};

#endif
