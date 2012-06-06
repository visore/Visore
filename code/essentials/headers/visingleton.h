#ifndef VISINGELTON_H
#define VISINGELTON_H

#include <QString>
#include <QSharedPointer>

class ViSingleton : public QObject
{

	public:

		static ViSingleton* instance();
		virtual QString name();

	protected:

		ViSingleton();

	protected:

		QString mName;

};

#endif
