#include "vitheme.h"

ViTheme::ViTheme()
{
	mName = "";
	mColors = NULL;
	mFonts = NULL;
}

ViTheme::~ViTheme()
{
	if(mColors != NULL)
	{
		delete mColors;
		mColors = NULL;
	}
	if(mFonts != NULL)
	{
		delete mFonts;
		mFonts = NULL;
	}
}

QString ViTheme::name()
{
	return mName;
}

ViThemeColors* ViTheme::colors()
{
	return mColors;
}

ViThemeFonts* ViTheme::fonts()
{
	return mFonts;
}
