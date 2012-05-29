#include "vitheme.h"

QSharedPointer<ViTheme> ViTheme::mInstance;

ViTheme::ViTheme()
{
	mName = "";
	mColors = NULL;
}

ViTheme::~ViTheme()
{
	if(mColors != NULL)
	{
		delete mColors;
		mColors = NULL;
	}
}

ViTheme* ViTheme::instance()
{
	return NULL;
}

QString ViTheme::name()
{
	return mName;
}

ViThemeColors* ViTheme::colors()
{
	return mColors;
}
