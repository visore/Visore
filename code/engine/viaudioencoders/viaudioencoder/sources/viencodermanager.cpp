#include "viencodermanager.h"

QSharedPointer<ViEncoderManager> ViEncoderManager::mInstance;

ViEncoderManager::ViEncoderManager()
{
	QList<QString> libraries = ViLibraryDetector::detectLibraries(VIAUDIOENCODERSLOCATION);
	for(int i = 0; i < libraries.length(); ++i)
	{			
		ViLibrary<ViAudioEncoder> *library = new ViLibrary<ViAudioEncoder>();
		if(library->open(libraries[i]))
		{
			mLibraries.append(library);
			mEncoders.append(library->createObject("createEncoder"));
		}
	}
}

ViEncoderManager::~ViEncoderManager()
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

ViEncoderManager* ViEncoderManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViEncoderManager>(new ViEncoderManager);
	}
	return mInstance.data();
}

QList<ViAudioEncoder*> ViEncoderManager::encoder(ViAudioFormat *format)
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	QList<ViAudioEncoder*> result;
	for(int i = 0; i < manager->mEncoders.size(); ++i)
	{
		if(format == manager->mEncoders[i]->format())
		{
			result.append(manager->mEncoders[i]);
		}
	}
	return result;
}

ViAudioEncoder* ViEncoderManager::encoder(QString name)
{
	ViEncoderManager *manager = ViEncoderManager::instance();
	for(int i = 0; i < manager->mEncoders.size(); ++i)
	{
		if(name == manager->mEncoders[i]->name())
		{
			return manager->mEncoders[i];
		}
	}
	return NULL;
}
