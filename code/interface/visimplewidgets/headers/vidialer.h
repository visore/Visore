#ifndef VIDIALER_H
#define VIDIALER_H

#include <QDial>
#include <QPainter>
#include <QLinearGradient>
#include "vigradientcreator.h"

class ViDialer : public QDial
{
	Q_OBJECT

	public:
		ViDialer(QWidget *parent = 0);
		void setSize(int width, int height);

	protected:
		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:
		bool mIsHover;
};

#endif
