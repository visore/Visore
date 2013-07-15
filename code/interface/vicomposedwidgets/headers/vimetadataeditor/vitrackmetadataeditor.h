#ifndef VITRACKMETADATAEDITOR_H
#define VITRACKMETADATAEDITOR_H

#include <viwidget.h>

namespace Ui
{
	class ViTrackMetadataEditor;
}

class ViTrackMetadataEditor : public ViWidget
{

	Q_OBJECT

	signals:

		void changed();

	public slots:

		void setTitle(const QString &title);
		void setArtist(const QString &artist);
		void setAlbum(const QString &album);

	private slots:

		bool previousImage();
		bool nextImage();
		void browseImage();
		void changeImage();

	public:

		ViTrackMetadataEditor(QWidget *parent = 0);
		~ViTrackMetadataEditor();

		void clear();

		void setMetadata(ViMetadata *metadata);
		bool hasMetadata();

		QString addPossibleImage(QString path);
		void setPossibleImages(QList<QString> images);
		void setPossibleImages(QList<ViMetadata> metadatas);

	private:

		bool hasImage(QString path);
		bool hasImage(QString path, QString &existingPath);

	private:

		Ui::ViTrackMetadataEditor *mUi;
		ViMetadata *mMetadata;
		QList<QString> mImages;
		QString mCurrentImage;
		int mCurrentIndex;

};

#endif
