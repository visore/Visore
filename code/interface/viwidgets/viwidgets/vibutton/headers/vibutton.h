#ifndef VIBUTTON_H
#define VIBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QLinearGradient>
#include "vigradientcreator.h"

class ViButton : public QWidget
{
	Q_OBJECT

	signals:
		void clicked(bool checked = false);

	public:
		ViButton(QWidget *parent = 0);
		~ViButton();
		void setSize(int width, int height);
		void setCheckable();
		void setChecked(bool checked);
		void setNormalIcon(QImage image);
		void setCheckedIcon(QImage image);

	protected:
		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		QImage mNormalImage;
		QImage mCheckedImage;
		bool mIsHover;
		bool mIsCheckable;
		bool mIsChecked;
		bool mIsPressed;
};

#endif
