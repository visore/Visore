#include <viserializer.h>
#include <QTextStream>

bool ViSerializer::exportData(QString filePath)
{
	QFile file(filePath);
	return exportData(file);
}

bool ViSerializer::exportData(QFile &file)
{
	bool openAgain = file.isOpen();
	QIODevice::OpenMode openMode;
	if(openAgain)
	{
		openMode = file.openMode();
		file.close();
	}

	bool success = false;
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << exportData().toXml();
		file.close();
		success = true;
	}

	if(openAgain)
	{
		file.open(openMode);
	}
	return success;
}

bool ViSerializer::importData(QString filePath)
{
	QFile file(filePath);
	return importData(file);
}

bool ViSerializer::importData(QFile &file)
{
	bool openAgain = file.isOpen();
	QIODevice::OpenMode openMode;
	if(openAgain)
	{
		openMode = file.openMode();
		file.close();
	}

	bool success = false;
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString result = "";
		QTextStream stream(&file);
		while(!stream.atEnd())
		{
			result += stream.readLine();
		}
		ViElement element;
		element.fromXml(result);
		success = !element.isNull();
		if(success)
		{
			success = importData(element);
		}
		file.close();
	}

	if(openAgain)
	{
		file.open(openMode);
	}
	return success;
}
