#ifndef VIPROJECTINFO_H
#define VIPROJECTINFO_H

#include <QMap>
#include <QVariant>

typedef QMap<QString, QVariant> ViInfoMap;

class ViProjectInfo : public ViInfoMap
{

	public:

		ViProjectInfo(QString name = "");
		void setName(QString name);
		QString name();

	private:

		QString mName;

};

#endif
