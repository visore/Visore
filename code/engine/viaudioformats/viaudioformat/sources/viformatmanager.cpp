#include "viformatmanager.h"

QSharedPointer<ViFormatManager> ViFormatManager::mInstance;

ViFormatManager::ViFormatManager()
{
	QList<QString> libraries = ViLibraryDetector::detectLibraries(VIAUDIOFORMATSLOCATION);
	for(int i = 0; i < libraries.length(); ++i)
	{			
		ViLibrary<ViAudioFormat> *library = new ViLibrary<ViAudioFormat>();
		if(library->open(libraries[i]))
		{
			mLibraries.append(library);
			mFormats.append(library->createObject("createFormat"));
		}
	}
}

ViFormatManager::~ViFormatManager()
{
	for(int i = 0; i < mLibraries.size(); ++i)
	{
		if(mLibraries[i] != NULL)
		{
			delete mLibraries[i];
			mLibraries[i] = NULL;
		}
	}
}

ViFormatManager* ViFormatManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViFormatManager>(new ViFormatManager());
	}
	return mInstance.data();
}

ViAudioFormat* ViFormatManager::format(QString name)
{
	ViFormatManager *manager = ViFormatManager::instance();
	for(int i = 0; i < manager->mFormats.length(); ++i)
	{
		if(manager->mFormats[i]->name() == name)
		{
			return manager->mFormats[i];
		}
	}
}

QList<ViAudioFormat*> ViFormatManager::formats(QList<QString> names)
{
	ViFormatManager *manager = ViFormatManager::instance();
	QList<ViAudioFormat*> formats;
	for(int i = 0; i < manager->mFormats.length(); ++i)
	{
		for(int j = 0; j < names.length(); ++j)
		{
			if(names[i] == manager->mFormats[i]->name())
			{
				formats.append(manager->mFormats[i]);
			}
		}
	}
	return formats;
}
