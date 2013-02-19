#ifndef VITHEMEANIMATION_H
#define VITHEMEANIMATION_H

#include <vithemeresource.h>

typedef QList<QImage> ViImageList;
typedef QList<QIcon> ViIconList;
typedef QList<QPixmap> ViPixmapList;

class ViThemeAnimation : public ViThemeResource
{

	public:

		ViThemeAnimation(QString name = "");
		
		QStringList paths();
		ViImageList images(int size = -1);
		ViIconList icons(int size = -1);
		ViPixmapList pixmaps(int size = -1);

	private:

		QStringList mPaths;

};

#endif
