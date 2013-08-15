#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "viaudioobject.h"
#include "viaudiocoder.h"
#include <QMap>

class ViProject;

typedef QQueue<ViProject*> ViProjectQueue;
typedef QList<ViProject*> ViProjectList;

class ViProject : public QObject, public ViId
{

	Q_OBJECT

	private:

		enum Directory
		{
			Root,

			Data,
			TargetData,
			CorruptedData,
			CorrectedData,
			NoiseData,
			CoverData,

			Info,
			GeneralInfo,
			TrackInfo,
			CorrectionInfo
		};

		enum File
		{
			Properties,
			Tracks
		};

	signals:

		void saved();
		void loaded();
		void finished();
        void cleared();

	public slots:

        /*******************************************************************************************************************

            CLEAR

        *******************************************************************************************************************/

        void clear(); //Removes project files and clears memory
        void clearObjects();
        void clearFiles();
        void clearCorrections();
        void clearCorrelations();

        /*******************************************************************************************************************

            LOAD & SAVE

        *******************************************************************************************************************/

		bool load(bool minimal = false); // minimal - Only load the vml files
		void save();

	private slots:

		bool loadAll();
		void saveAll();
		void setFinished();
		void setBusy();

	public:

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		ViProject(QString filePath = "");
		ViProject(QString projectName, QString filePath, int sides);
		ViProject(const ViProject &other);
		~ViProject();

		/*******************************************************************************************************************

			OBJECTS

		*******************************************************************************************************************/

		int objectIndex(const QString &id);
		int objectIndex(ViAudioObjectPointer object);
		int objectCount();
		ViAudioObjectQueue objects();
		ViAudioObjectPointer object(int index);	
		ViAudioObjectPointer object(int side, int track);
		void add(ViAudioObjectPointer object);

		/*******************************************************************************************************************

			BASICS

		*******************************************************************************************************************/

		qint64 size();

		void setFilePath(QString filePath);
		QString filePath() const;
		QString fileName() const;

		bool isFinished();

        /*******************************************************************************************************************

            CORRELATIONS

        *******************************************************************************************************************/

		ViCorrelation bestCorrelation(QString correlator, int songIndex = -1);
		qreal bestImprovement(QString correlator, int songIndex = -1);
		QString bestCorrectionId(QString correlator, int songIndex = -1);
        QString currentCorrectionId();

        /*******************************************************************************************************************

            SIDES & TRACKS

        *******************************************************************************************************************/

		int sideCount();
		int trackCount();
		int trackCount(int side);

		/*******************************************************************************************************************

			PROPERTIES

		*******************************************************************************************************************/

		void setProjectName(QString name);
		QString projectName();

		ViVersion createdVersion();
		ViVersion editedVersion();

	protected:

		/*******************************************************************************************************************

			DIRECTORIES & FILES

		*******************************************************************************************************************/

		bool initializeStructure();
		bool createStructure();
		bool removeStructure();

		bool createDirectory(QString directoryPath);
		bool createDirectory(int side);
		bool createDirectory(ViProject::Directory directory, int side);
		bool removeDirectory(QString directoryPath);

		bool createFile(QString filePath);
		bool removeFile(QString filePath);

		QString relativePath(QString path);
		QString absolutePath(QString path);

		QString path(ViProject::Directory directory);
		QString path(ViProject::File file);
		QString path(ViProject::Directory directory, int side);
		QString path(ViAudio::Type type, int side);

		QStringList fileNames(bool track = true, bool side = false);

		QStringList files(const QString &directory) const;
		void cleanProject();
		void moveToProject();
		void moveToProject(ViAudioObjectPointer object, ViAudio::Type type);

        /*******************************************************************************************************************

            CORRECTIONS

        *******************************************************************************************************************/

		void bestCorrection(const QString &fileId, const ViCorrelationGroup &group1, const ViCorrelationGroup &group2, QHash<QString, ViCorrelation> &bestCorrelation, QHash<QString, QString> &bestId, QHash<QString, qreal> &bestImprovement);
        QString nextCorrectionId();
        QString correctionId(QString path = "");
        QString correctionPath(QString id = "");

		/*******************************************************************************************************************

			SAVE & LOAD INFORMATION

		*******************************************************************************************************************/

		bool saveProperties();
		bool saveTracks();
        bool saveCorrections();

		bool loadProperties();
		bool loadTracks();
        bool loadCorrections();

	private:

		bool mFinished;

		QMap<ViProject::Directory, QString> mPaths;
		QMap<ViProject::File, QString> mFiles;

		ViArchive mArchive;

		ViAudioObjectQueue mObjects;

		QMutex mFinishedMutex;
        QMutex mObjectsMutex;

        /*******************************************************************************************************************

            CORRELATIONS

        *******************************************************************************************************************/

		QString mCurrentCorrectionId;

		ViCorrelationGroup mBestCorrelation;
        QHash<QString, qreal> mBestImprovement;
        QHash<QString, QString> mBestCorrectionId;

		QHash<int, ViCorrelationGroup> mBestTrackCorrelation;
		QHash<int, QHash<QString, qreal>> mBestTrackImprovement;
		QHash<int, QHash<QString, QString>> mBestTrackCorrectionId;

		/*******************************************************************************************************************

			PROPERTIES

		*******************************************************************************************************************/

		QString mProjectName;
		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
};

#endif
