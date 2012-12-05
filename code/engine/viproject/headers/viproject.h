#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "viaudioobject.h"
#include "viaudiocoder.h"
#include <QMap>

class ViProject : public QObject, public ViId
{

	Q_OBJECT

	public:

		enum Mode
		{
			Target,
			Oringinal,
			Corrected,
			Correlation = Corrected
		};

	signals:

		void finished();

	public slots:

		void serialize(ViAudioObjectPointer object, ViAudio::Type type);
		void setFormat(ViAudioFormat format);

		/*******************************************************************************************************************

			LOAD & SAVE

		*******************************************************************************************************************/

		bool load(bool minimal = false); // minimal - Only load the vml files
		void save();

	private slots:

		bool loadAll();
		void saveAll();

	public:

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		ViProject(QString filePath);
		ViProject(QString projectName, QString filePath, int sides);
		~ViProject();

		/*******************************************************************************************************************

			BASICS

		*******************************************************************************************************************/

		qint64 size();

		void setFilePath(QString filePath);
		QString filePath();

		/*******************************************************************************************************************

			SIDES

		*******************************************************************************************************************/

		void setSides(int sides);
		int sides();
		int currentSide();
		void nextSide();

		/*******************************************************************************************************************

			PROPERTIES

		*******************************************************************************************************************/

		void setProjectName(QString name);
		QString projectName();

		ViVersion createdVersion();
		ViVersion editedVersion();

	protected:

		bool createTempStructure();
		bool removeTempStructure();
		bool createSideStructure();
		bool removeSideStructure();

		QString generateFileName(ViSongInfo info, QString folder, QString extension);

		QString relativePath(QString path);
		QString absolutePath(QString path);

		void clearObjects();

		/*******************************************************************************************************************

			SAVE & LOAD INFORMATION

		*******************************************************************************************************************/

		bool saveProperties();
		bool saveTracks();
		bool saveCorrelations();

		bool loadProperties();
		bool loadTracks();
		bool loadCorrelations();

	private:

		int mSides;
		int mCurrentSide;
		int mCurrentTrack;

		QMap<QString,QString> mPaths;
		QMap<QString,QString> mFiles;

		ViArchive mArchive;
		ViAudioFormat mFormat;
		ViAudioCoder mEncoder;

		QList<QList<ViAudioObjectPointer> > mObjects;

		/*******************************************************************************************************************

			PROPERTIES

		*******************************************************************************************************************/

		QString mProjectName;
		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
};

#endif
