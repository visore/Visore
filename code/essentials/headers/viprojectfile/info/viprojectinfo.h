#ifndef VIPROJECTINFO_H
#define VIPROJECTINFO_H

#include "viinfoelement.h"

class ViProjectInfo
{

	public:

		ViProjectInfo();

		void setDirectory(QString directory);
		QString name();
		bool isNull();

		bool load();
		bool save();

	protected:

		virtual ViInfoElement toXml() = 0;
		virtual bool fromXml(ViInfoElement &document) = 0;

		void setName(QString name);
		void setNull(bool null);

	private:

		bool mIsNull;
		QString mName;
		QString mFilePath;

};

#endif
