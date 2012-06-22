#ifndef VIID_H
#define VIID_H

#include <QList>
#include <QString>
#include <QSharedPointer>

#include<iostream>
using namespace std;

class ViId
{

	public:

		ViId();
		~ViId();
		QString id() const;
		bool setId(QString id);
		bool operator ==(const ViId &other) const;
		bool operator !=(const ViId &other) const;

	protected:

		void createId();

	private:

		QString mId;

};

class ViIdManager
{

	public:

		static QString generate();
		static void release(QString id);

	protected:

		static ViIdManager* instance();
		static QString random();

	protected:

		static QSharedPointer<ViIdManager> mInstance;
		QList<QString> mIds;

};

#endif
