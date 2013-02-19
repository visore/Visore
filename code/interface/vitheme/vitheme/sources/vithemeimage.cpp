#include <vithemeimage.h>

ViThemeImage::ViThemeImage(QString name)
	: ViThemeResource()
{
	setDirectory(":/images/" + name);
	mPath = generatePath();
}

ViThemeImage::ViThemeImage(const ViThemeImage &other)
	: ViThemeResource(other)
{
	mPath = other.mPath;
}

QIcon ViThemeImage::icon(int size)
{
	if(size < 0)
	{
		return QIcon(path());
	}
	return QIcon(pixmap(size));
}

QImage ViThemeImage::image(int size)
{
	if(size < 0)
	{
		return QImage(path());
	}
	return QImage(path()).scaledToWidth(size, Qt::SmoothTransformation);
}

QPixmap ViThemeImage::pixmap(int size)
{
	if(size < 0)
	{
		return QPixmap(path());
	}
	return QPixmap::fromImage(image(size));
}

QString ViThemeImage::path()
{
	return mPath;
}
