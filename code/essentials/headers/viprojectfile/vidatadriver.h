#ifndef VIDATADRIVER_H
#define VIDATADRIVER_H

#include "viabstractinfo.h"
#include <QSqlQuery>
#include <QSqlDatabase>

class ViDataDriver
{

	public:

		void save(ViAbstractInfo &info);
		void save(ViProjectInfo &info);

		void load(ViAbstractInfo &info);
		void load(ViProjectInfo &info);
		ViProjectInfo load();

	protected:

		virtual QSqlQuery insertQuery(ViProjectInfo &info);
		virtual QSqlQuery selectQuery(ViProjectInfo &info);

	protected:

		QSqlDatabase mDatabase;

};

#endif
