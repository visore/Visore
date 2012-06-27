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

		void setFilePath(QString filePath);
		QString filePath();

		void load();
		void save();

		//Properties
		ViVersion createdVersion();
		ViVersion editedVersion();

	protected:

		bool createTempStructure();
		bool removeTempStructure();

	private:

		QString mTempPath;
		QString mDataPath;

		ViArchive mArchive;

		QList<ViProjectFile*> mProjectFiles;
		ViPropertiesFile mProperties;

};

#endif
