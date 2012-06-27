#ifndef VIPROJECTFILE_H
#define VIPROJECTFILE_H

#include <QDomDocument>

class ViProjectFile
{

	public:

		ViProjectFile();

		void setDirectory(QString directory);
		QString name();
		bool isNull();

		bool load();
		bool save();

	protected:

		virtual QDomDocument toXml() = 0;
		virtual void fromXml(QDomDocument document) = 0;

		void setName(QString name);
		void setNull(bool null);

	private:

		bool mIsNull;
		QString mName;
		QString mFilePath;

};

#endif
