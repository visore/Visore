#ifndef VISINGELTON_H
#define VISINGELTON_H

#include <QString>
#include <QSharedPointer>

class ViSingleton
{
	public:
		static ViSingleton* instance();
		virtual QString name() = 0;

	protected:
		ViSingleton();

	protected:
		QString mName;
};

#endif
