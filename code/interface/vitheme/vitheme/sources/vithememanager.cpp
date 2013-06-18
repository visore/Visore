#include "vithememanager.h"
#include "vilogger.h"

ViThemeManager::ViThemeManager()
{
	bool error = false;
	mLibrary.setFileName("./interface/themes/vivulcanotheme");
	if(mLibrary.load())
	{
		typedef ViThemePointer (*CreateFunction)();
		CreateFunction createFunction = (CreateFunction) mLibrary.resolve("createTheme");
		if(createFunction)
		{
			mCurrentTheme = createFunction();
			if(!mCurrentTheme.isNull())
			{
				LOG(mCurrentTheme->name() + " theme loaded.");
			}
			else
			{
				error = true;
			}
		}
		else
		{
			error = true;
		}
	}
	else
	{
		error = true;
	}
	if(error)
	{
		mLibrary.unload();
		LOG("No theme could be loaded.", QtFatalMsg);
	}
}

ViThemeManager::~ViThemeManager()
{
    mLibrary.unload();
}

QColor ViThemeManager::color(ViThemeColors::Type type)
{
	return ViThemeManager::instance()->mCurrentTheme->colors()->color(type);
}

ViFont ViThemeManager::font(ViThemeFonts::Type type)
{
	return ViThemeManager::instance()->mCurrentTheme->fonts()->font(type);
}

ViThemeImage ViThemeManager::image(QString name)
{
	ViThemeManager::instance();
	return ViThemeImage(name);
}

ViThemeIcon ViThemeManager::icon(QString name)
{
	ViThemeManager::instance();
	return ViThemeIcon(name);
}

ViThemeAnimation ViThemeManager::animation(QString name)
{
	ViThemeManager::instance();
	return ViThemeAnimation(name);
}
