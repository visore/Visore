#ifndef VISERIALIZER_H
#define VISERIALIZER_H

#include <vielement.h>
#include <QFile>

class ViSerializer
{

	public:

        virtual ViElement exportData();
        virtual bool importData(ViElement element);

		virtual bool exportFile(QString filePath);
		virtual bool exportFile(QFile &file);

		virtual bool importFile(QString filePath);
		virtual bool importFile(QFile &file);

};

#endif
