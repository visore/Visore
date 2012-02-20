#include "vithememanager.h"

QSharedPointer<ViThemeManager> ViThemeManager::mInstance;

ViThemeManager::ViThemeManager()
{
	QList<QString> libraries = ViLibraryDetector::detectLibraries(VITHEMESLOCATION);
	for(int i = 0; i < libraries.length(); ++i)
	{			
		ViLibrary<ViTheme> *library = new ViLibrary<ViTheme>();
		if(library->open(libraries[i]))
		{
			mLibraries.append(library);
			mThemes.append(library->createObject("createTheme"));
		}
	}
	mCurrentTheme = mThemes[0];
}

ViThemeManager::~ViThemeManager()
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

ViThemeManager* ViThemeManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViThemeManager>(new ViThemeManager());
	}
	return mInstance.data();
}

QList<QString> ViThemeManager::themes()
{
	ViThemeManager *manager = ViThemeManager::instance();
	QList<QString> result;
	for(int i = 0; i < manager->mThemes.length(); ++i)
	{
		result.append(manager->mThemes[i]->name());
	}
	return result;
}

void ViThemeManager::setTheme(QString name)
{
	ViThemeManager *manager = ViThemeManager::instance();
	for(int i = 0; i < manager->mThemes.length(); ++i)
	{
		if(name == manager->mThemes[i]->name())
		{
			manager->mCurrentTheme = manager->mThemes[i];
			return;
		}
	}
}

QColor ViThemeManager::color(int index)
{
	ViThemeManager *manager = ViThemeManager::instance();
	return manager->mCurrentTheme->colors()->color(index);
}
