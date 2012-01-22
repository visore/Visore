#include "viobject.h"

void ViObject::connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
{
	//Only connect a slot if it isn't already connected
	QObject::connect(sender, signal, receiver, method, Qt::UniqueConnection);
}

void ViObject::disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
{
	//Only connect a slot if it isn't already connected
	QObject::disconnect(sender, signal, receiver, method);
}

void ViObject::connectDirect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
{
	//Only connect a slot if it isn't already connected
	QObject::connect(sender, signal, receiver, method, Qt::DirectConnection);
}
