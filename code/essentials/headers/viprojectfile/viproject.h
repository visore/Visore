#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "viprojectproperties.h"
#include "viprojectsongs.h"

class ViProject : public QObject, public ViId
{
	Q_OBJECT

	signals:

		void finished();

	private slots:

		void readInfo();

	public:

		ViProject(QString filePath = "", QString projectName = "");
		~ViProject();

		void setFilePath(QString filePath);
		QString filePath();
		QString fileName(bool withExtension = true);

		bool load(bool minimal = false); // minimal - Only load the vml files
		void save();
		qint64 size();

		int sideCount();
		int currentSide();
		void setSideCount(int count);
		bool nextSide();

		//Properties
		void setProjectName(QString name);
		QString projectName();
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

		int mSideCount;
		int mCurrentSide;
		QString mTempPath;
		QString mInfoPath;
		QString mDataPath;
		QString mOriginalPath;
		QString mCorrectedPath;
		QString mAlbumArtPath;

		ViArchive mArchive;

		QList<ViProjectInfo*> mProjectInfos;
		ViProjectProperties mProperties;
		ViProjectSongs mSongs;

};

#endif
