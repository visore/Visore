#ifndef VISERIALIZER_H
#define VISERIALIZER_H

#include <vielement.h>
#include <QFile>

class ViSerializer
{

	public:

		virtual ViElement exportData() = 0;
		virtual bool importData(ViElement element) = 0;

		virtual bool exportData(QString filePath);
		virtual bool exportData(QFile &file);

		virtual bool importData(QString filePath);
		virtual bool importData(QFile &file);

};

#endif
