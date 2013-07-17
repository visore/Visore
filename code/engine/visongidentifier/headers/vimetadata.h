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

		void clear();
		bool isValid() const;

		void setArtist(const QString &artist);
		void setTitle(const QString &title);
		void setAlbum(const QString &album);
		void setCover(const QString &cover);

		QString artist(const bool &returnDefault = true) const;
		QString title(const bool &returnDefault = true) const;
		QString album(const bool &returnDefault = true) const;
		QString cover() const;

		bool hasArtist() const;
		bool hasTitle() const;
		bool hasAlbum() const;
		bool hasCover() const;

		QImage coverImage() const;
		QByteArray coverData() const;
		QString coverFormat() const;
		QString coverMimeType() const;
		bool copyCover(const QString &newPath, const bool &deleteOld = false); // Copy the current cover to the new location

		QString toString() const;
		QString toShortString() const;

		static QString unknownArtist();
		static QString unknownTitle();
		static QString unknownAlbum();

		bool operator == (const ViMetadata &other) const;
		bool operator != (const ViMetadata &other) const;

	private:

		QString mArtist;
		QString mTitle;
		QString mAlbum;
		QString mCover;

};

#endif
