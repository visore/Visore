#ifndef VITOOLBUTTON_H
#define VITOOLBUTTON_H

#include <QPainter>
#include "viabstractbutton.h"
#include "vithemeimage.h"

class ViToolButton : public ViAbstractButton
{
	Q_OBJECT

	public:

		ViToolButton(QWidget *parent = 0);
		void setIcon(ViThemeImage image, ViThemeImage::State state);

	protected:

		void paintEvent(QPaintEvent *event);

	private:

		QImage mNormalImage;
		QImage mHoveredImage;
		QImage mSelectedImage;
		QImage mDisbaledImage;

};

#endif
