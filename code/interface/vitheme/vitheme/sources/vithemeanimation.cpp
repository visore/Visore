#include <vithemeanimation.h>

ViThemeAnimation::ViThemeAnimation(QString name)
	: ViThemeResource()
{
	setDirectory(":/animations/" + name);
	QString path;
	int counter = 1;
	do
	{
		path = generatePath(QString::number(counter));
		if(path == "")
		{
			break;
		}
		else
		{
			++counter;
			mPaths.append(path);
		}
	}
	while(true);
}
		
QStringList ViThemeAnimation::paths()
{
	return mPaths;
}

ViImageList ViThemeAnimation::images(int size)
{
	ViImageList result;
	if(size < 0)
	{
		for(int i = 0; i < mPaths.size(); ++i)
		{
			result.append(QImage(mPaths[i]));
		}
	}
	else
	{
		for(int i = 0; i < mPaths.size(); ++i)
		{
			result.append(QImage(mPaths[i]).scaledToWidth(size, Qt::SmoothTransformation));
		}
	}
	return result;
}

ViIconList ViThemeAnimation::icons(int size)
{
	ViPixmapList pixmap = pixmaps(size);
	ViIconList result;
	for(int i = 0; i < pixmap.size(); ++i)
	{
		result.append(QIcon(pixmap[i]));
	}
	return result;
}

ViPixmapList ViThemeAnimation::pixmaps(int size)
{
	ViImageList image = images(size);
	ViPixmapList result;
	for(int i = 0; i < image.size(); ++i)
	{
		result.append(QPixmap::fromImage(image[i]));
	}
	return result;
}
