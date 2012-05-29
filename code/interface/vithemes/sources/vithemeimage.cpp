#include "vithemeimage.h"

ViThemeImage::ViThemeImage(QString path, ViThemeImage::State state)
{
	mPath = path;
	mState = state;
}

QString ViThemeImage::path()
{
	return mPath;
}

QImage ViThemeImage::image()
{
	return QImage(mPath);
}

QIcon ViThemeImage::icon()
{
	return QIcon(mPath);
}

ViThemeImage::State ViThemeImage::state()
{
	return mState;
}
