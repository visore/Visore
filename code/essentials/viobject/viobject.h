#ifndef VIOBJECT_H
#define VIOBJECT_H

#include <QObject>

class ViObject
{
	public:
		static void connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
		{
			//Only connect a slot if it isn't already connected
			QObject::connect(sender, signal, receiver, method, Qt::UniqueConnection);
		}
		static void disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
		{
			//Only connect a slot if it isn't already connected
			QObject::disconnect(sender, signal, receiver, method);
		}
};

#endif
