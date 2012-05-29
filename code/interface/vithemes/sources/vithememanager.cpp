#include "vithememanager.h"
#include "viorangetheme.h"

QSharedPointer<ViThemeManager> ViThemeManager::mInstance;

ViThemeManager::ViThemeManager()
{
	mCurrentTheme = ViOrangeTheme::instance();
}

ViThemeManager::~ViThemeManager()
{
	delete mCurrentTheme;
}

ViThemeManager* ViThemeManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViThemeManager>(new ViThemeManager());
	}
	return mInstance.data();
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
	else if(type == ViThemeManager::Animation)
	{
		prefix = ":/animations/";
	}
	ViThemeImage image(prefix + statePrefix + name, state);
	return image;
}
