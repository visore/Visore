#ifndef VITHEMEIMAGE_H
#define VITHEMEIMAGE_H

#include <vithemeresource.h>

class ViThemeImage : public ViThemeResource
{

	public:

		ViThemeImage(QString name = "");
		ViThemeImage(const ViThemeImage &other);
		
		QIcon icon(int size = -1);
		QImage image(int size = -1);
		QPixmap pixmap(int size = -1);

		QString path();

	private:

		QString mPath;

};

#endif
