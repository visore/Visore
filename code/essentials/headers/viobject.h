#ifndef VIOBJECT_H
#define VIOBJECT_H

#include <QObject>

class ViObject
{
	public:
		static void connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method);
		static void disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method);
};

#endif
