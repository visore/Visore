#ifndef VIAUDIOFORMATMANAGER_H
#define VIAUDIOFORMATMANAGER_H

#include "viaudioformat.h"
#include "viessentials.h"

class ViAudioFormatManager
{
	public:
		static ViAudioFormatManager* initialize()
		{
			if(mInstance == NULL)
			{
				mInstance = new ViAudioFormatManager();
			}
			return mInstance;
		}

		static void close()
		{
			if(mInstance != NULL)
			{
				delete mInstance;
				mInstance = NULL;
			}
		}

		ViAudioFormat* format(QString name)
		{
			for(int i = 0; i < mFormats.length(); ++i)
			{
				if(mFormats[i]->name() == name)
				{
					return mFormats[i];
				}
			}
		}

		QList<ViAudioFormat*>* formats(QList<QString> names)
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

	protected:
		ViAudioFormatManager()
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

		~ViAudioFormatManager()
		{
			for(int i = 0; i < mLibraries.length(); ++i)
			{
				mLibraries[i]->deleteObject("deleteFormat", mFormats[i]);
				delete mLibraries[i];
				mLibraries[i] = NULL;
			}
		}

	private:
		static ViAudioFormatManager *mInstance;
		QList<ViLibrary<ViAudioFormat>*> mLibraries;
		QList<ViAudioFormat*> mFormats;
};

#endif

ViAudioFormatManager* ViAudioFormatManager::mInstance = NULL;
