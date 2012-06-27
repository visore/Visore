#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "vipropertiesfile.h"

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

		//Recordings
		QString nextSongName();
		QString originalPath();
		QString correctedPath();

	protected:

		bool createTempStructure();
		bool removeTempStructure();

		QString padName(int number);

	private:

		QString mTempPath;
		QString mDataPath;
		QString mOriginalPath;
		QString mCorrectedPath;

		ViArchive mArchive;

		QList<ViProjectFile*> mProjectFiles;
		ViPropertiesFile mProperties;

};

#endif
