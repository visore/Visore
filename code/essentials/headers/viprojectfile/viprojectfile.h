#ifndef VIPROJECTFILE_H
#define VIPROJECTFILE_H

#include "vidatadriver.h"
#include "viarchive.h"

class ViProjectFile : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	private slots:

		void completeArchive();

	public:

		ViProjectFile(QString filePath = "");
		void setFilePath(QString filePath);
		QString filePath();
		void load();
		void save();

		ViPropertiesInfo& properties();

	protected:

		QString fileName();
		void completeSave();
		void completeLoad();

	private:

		void (ViProjectFile::*complete)();

		QString mFilePath;
		QString mProjectName;
		QString mProjectTempPath;
		ViPropertiesInfo mProperties;
		ViArchive mArchive;

};

#endif
