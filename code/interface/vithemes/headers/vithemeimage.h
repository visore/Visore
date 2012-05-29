#ifndef VITHEMEIMAGE_H
#define VITHEMEIMAGE_H

#include <QString>
#include <QIcon>
#include <QImage>

class ViThemeImage
{
	public:
		enum State
		{
			Normal = 0,
			Selected = 1,
			Disabled = 2,
			Hovered = 3
		};
		ViThemeImage(QString path, ViThemeImage::State state);
		QString path();
		QImage image();
		QIcon icon();
		ViThemeImage::State state();

	private:
		QString mPath;
		ViThemeImage::State mState;
};

#endif
