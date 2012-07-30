#ifndef VIFILESONGINFO_H
#define VIFILESONGINFO_H

#include <visonginfo.h>

class ViFileSongInfo : public ViSongInfo
{
	public:

		ViFileSongInfo();
		ViFileSongInfo(const ViSongInfo &other);
		ViFileSongInfo(const ViFileSongInfo &other);

		QString originalFilePath();
		QString correctedFilePath();
		QString albumArtPath();
		QImage albumArtImage();

		void setOriginalFilePath(QString path);
		void setCorrectedFilePath(QString path);
		void setAlbumArtPath(QString path);

	private:

		QString mOriginalFilePath;
		QString mCorrectedFilePath;
		
};

typedef QList<ViFileSongInfo> ViFileSongInfoList;

#endif
