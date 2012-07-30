#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "vipropertiesfile.h"
#include "visongfile.h"

class ViProject : public QObject, public ViId
{
	Q_OBJECT

	signals:

		void finished();

	private slots:

		void readInfo();

	public:

		ViProject(QString filePath = "");
		~ViProject();

		void setFilePath(QString filePath);
		QString filePath();

		void load();
		void save();

		//Properties
		ViVersion createdVersion();
		ViVersion editedVersion();

		//Songs
		ViFileSongInfoList songs();
		void addSong(ViFileSongInfo info);

		//Recordings
		QString nextOriginalSongNumber();
		QString nextCorrectedSongNumber();
		QString originalPath();
		QString correctedPath();
		QString albumArtPath();

	protected:

		bool createTempStructure();
		bool removeTempStructure();

		QString nextSongNumber(QString path);
		QString padName(int number);

	private:

		QString mTempPath;
		QString mDataPath;
		QString mOriginalPath;
		QString mCorrectedPath;
		QString mAlbumArtPath;

		ViArchive mArchive;

		QList<ViProjectFile*> mProjectFiles;
		ViPropertiesFile mProperties;
		ViSongFile mSongs;

};

#endif
