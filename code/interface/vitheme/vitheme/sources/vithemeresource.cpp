#include <vithemeresource.h>
#include <QDir>
#include <QFile>
#include <QImageReader>

ViThemeResource::ViThemeResource()
{
	mDirectory = "";
	mName = "";
}

ViThemeResource::ViThemeResource(const ViThemeResource &other)
{
	mDirectory = other.mDirectory;
	mName = other.mName;
}

void ViThemeResource::setDirectory(QString directory)
{
	mDirectory = directory;
	if(!mDirectory.endsWith("/"))
	{
		mDirectory += "/";
	}
	QDir dir(mDirectory);
	mName = dir.dirName();
}

QString ViThemeResource::directory()
{
	return mDirectory;
}

QString ViThemeResource::name()
{
	return mName;
}

QString ViThemeResource::generatePath(QString sub)
{
	static QStringList extensions = formats();

	if(mDirectory == "" || mName == "" || ":/" + mName + "/" == mDirectory)
	{
		return "";
	}

	QString result = mDirectory + mName;
	if(sub != "")
	{
		result += "_" + sub;
	}
	result += ".";
	for(int i = 0; i < extensions.size(); ++i)
	{
		if(exists(result + extensions[i]))
		{
			result += extensions[i];
			break;
		}
	}
	if(result.endsWith("."))
	{
		result = "";
	}
	return result;
}

QStringList ViThemeResource::formats()
{
	QList<QByteArray> data = QImageReader::supportedImageFormats();
	QStringList result;
	for(int i = 0; i < data.size(); ++i)
	{
		result.append(QString(data[i]).toLower());
	}
	return result;
}

bool ViThemeResource::exists(QString path)
{
	QFile file(path);
	return file.exists();
}
