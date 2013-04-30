#include <viserializer.h>
#include <QTextStream>

bool ViSerializer::exportFile(QString filePath)
{
	QFile file(filePath);
	return exportFile(file);
}

bool ViSerializer::exportFile(QFile &file)
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

bool ViSerializer::importFile(QString filePath)
{
	QFile file(filePath);
	return importFile(file);
}

bool ViSerializer::importFile(QFile &file)
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
