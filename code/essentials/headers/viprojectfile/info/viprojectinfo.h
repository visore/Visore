#ifndef VIPROJECTINFO_H
#define VIPROJECTINFO_H

#include "vielement.h"

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

		virtual ViElement toXml() = 0;
		virtual bool fromXml(ViElement &document) = 0;

		void setName(QString name);
		void setNull(bool null);

	private:

		bool mIsNull;
		QString mName;
		QString mFilePath;

};

#endif
