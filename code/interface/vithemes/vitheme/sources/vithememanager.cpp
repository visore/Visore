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

QString ViThemeManager::stateString(ViThemeImage::State state)
{
	if(state ==ViThemeImage::Normal)
	{
		return "normal/";
	}
	else if(state == ViThemeImage::Selected)
	{
		return "selected/";
	}
	else if(state == ViThemeImage::Disabled)
	{
		return "disabled/";
	}
	return "";
}

QColor ViThemeManager::color(int index)
{
	ViThemeManager *manager = ViThemeManager::instance();
	return manager->mCurrentTheme->colors()->color(index);
}

ViThemeImage ViThemeManager::image(QString name, ViThemeImage::State state, ViThemeManager::Type type)
{
	ViThemeManager::instance();
	QString prefix = ":/";
	QString statePrefix = "";
	if(type == ViThemeManager::Icon)
	{
		prefix = ":/icons/";
		statePrefix = ViThemeManager::stateString(state);
	}
	else if(type == ViThemeManager::Background)
	{
		prefix = ":/backgrounds/";
	}
	ViThemeImage image(prefix + statePrefix + name, state);
	return image;
}
