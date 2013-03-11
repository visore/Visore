#ifndef VISERIALIZER_H
#define VISERIALIZER_H

#include <vielement.h>
#include <QFile>

class ViSerializer
{

	public:

		virtual ViElement exportData() = 0;
		virtual bool importData(ViElement element) = 0;

		virtual bool exportFile(QString filePath);
		virtual bool exportFile(QFile &file);

		virtual bool importFile(QString filePath);
		virtual bool importFile(QFile &file);

};

#endif
