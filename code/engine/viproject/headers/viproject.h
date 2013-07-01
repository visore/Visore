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
        void infoCorrector();

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

        ViCorrelation bestCorrelation(QString correlator);
        qreal bestImprovement(QString correlator);

        QString bestCorrectionId(QString correlator);
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

		void moveToProject();
		void moveToProject(ViAudioObjectPointer object, ViAudio::Type type);

        /*******************************************************************************************************************

            CORRECTIONS

        *******************************************************************************************************************/

        QString nextCorrectionId();
        QString correctionId(QString path = "");
        QString correctionPath(QString id = "");
        bool hasCorrector(ViAudioObjectPointer object);
        ViElement corrector(ViAudioObjectPointer object);

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

        QList<QPair<ViAudioObjectPointer, ViElement>> mCorrectors;
        QString mCurrentCorrectionId;

        QMutex mObjectsMutex;

        /*******************************************************************************************************************

            CORRELATIONS

        *******************************************************************************************************************/

        ViCorrelationGroup mBestCorrelation;
        QHash<QString, qreal> mBestImprovement;
        QHash<QString, QString> mBestCorrectionId;

		/*******************************************************************************************************************

			PROPERTIES

		*******************************************************************************************************************/

		QString mProjectName;
		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
};

#endif
