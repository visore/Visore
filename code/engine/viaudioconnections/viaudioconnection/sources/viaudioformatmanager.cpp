#include "viaudioformatmanager.h"

ViAudioFormatManager* ViAudioFormatManager::mInstance = NULL;

ViAudioFormatManager::ViAudioFormatManager()
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

ViAudioFormatManager::~ViAudioFormatManager()
{
	for(int i = 0; i < mLibraries.length(); ++i)
	{
		mLibraries[i]->deleteObject("deleteFormat", mFormats[i]);
		delete mLibraries[i];
		mLibraries[i] = NULL;
	}
}

ViAudioFormatManager* ViAudioFormatManager::initialize()
{
	if(mInstance == NULL)
	{
		mInstance = new ViAudioFormatManager();
	}
	return mInstance;
}

void ViAudioFormatManager::close()
{
	if(mInstance != NULL)
	{
		delete mInstance;
		mInstance = NULL;
	}
}

ViAudioFormat* ViAudioFormatManager::format(QString name)
{
	for(int i = 0; i < mFormats.length(); ++i)
	{
		if(mFormats[i]->name() == name)
		{
			return mFormats[i];
		}
	}
}

QList<ViAudioFormat*>* ViAudioFormatManager::formats(QList<QString> names)
{
	QList<ViAudioFormat*>* formats = new QList<ViAudioFormat*>();
	for(int i = 0; i < mFormats.length(); ++i)
	{
		for(int j = 0; j < names.length(); ++j)
		{
			if(names[i] == mFormats[i]->name())
			{
				formats->append(mFormats[i]);
			}
		}
	}
	return formats;
}
