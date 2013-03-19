#include <vilibrarymanager.h>
#include <vicommon.h>
#include <QDir>

ViLibraryManager::ViLibraryManager(QString directory)
{
	load(directory);
}

ViLibraryManager::~ViLibraryManager()
{
	unload();
}

int ViLibraryManager::load(QString directory)
{
	int loadCount = 0;
	QStringList libraries = find(directory);
	for(int i = 0; i < libraries.size(); ++i)
	{
		if(!contains(libraries[i]))
		{
			QLibrary *library = new QLibrary(libraries[i]);
			if(library->load())
			{
				++loadCount;
				mLibraries.append(library);
			}
		}
	}
	return loadCount;
}

void ViLibraryManager::unload()
{
	for(int i = 0; i < mLibraries.size(); ++i)
	{
		mLibraries[i]->unload();
	}
	viDeleteAll(mLibraries);
}

bool ViLibraryManager::contains(QLibrary *library)
{
	return contains(library->fileName());
}

bool ViLibraryManager::contains(QString libraryPath)
{
	libraryPath = libraryPath.trimmed();
	for(int i = 0; i < mLibraries.size(); ++i)
	{
		if(mLibraries[i]->fileName() == libraryPath)
		{
			return true;
		}
	}
	return false;
}

QList<QFunctionPointer> ViLibraryManager::resolve(QString functionName)
{
	QList<QFunctionPointer> pointers;
	for(int i = 0; i < mLibraries.size(); ++i)
	{
		QFunctionPointer pointer = mLibraries[i]->resolve(functionName.toLatin1().data());
		if(pointer != 0)
		{
			pointers.append(pointer);
		}
	}
	return pointers;
}

QStringList ViLibraryManager::find(QString directory)
{
	QDir dir(directory);
	QStringList libraries;
	QStringList allFiles = dir.entryList(QDir::Files);
	for(int i = 0; i < allFiles.size(); ++i)
	{
		if(QLibrary::isLibrary(allFiles[i]))
		{
			libraries.append(allFiles[i]);
		}
	}
	return libraries;
}
