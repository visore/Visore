#ifndef VIPROJECTFILE_H
#define VIPROJECTFILE_H

#include "vidatadriver.h"
#include "viarchive.h"

class ViProjectFile : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	public:

		ViProjectFile(QString filePath = "");
		void setFilePath(QString filePath);
		QString filePath();
		void load();
		void save();

		ViPropertiesInfo& properties();

		QString toString();

	protected:

		QString fileName();

	private:

		QString mFilePath;
		QString mProjectName;
		QString mProjectTempPath;
		ViPropertiesInfo mProperties;
		ViArchive mArchive;

};

#endif
