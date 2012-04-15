#ifndef VIPUSHBUTTON_H
#define VIPUSHBUTTON_H

#include <QPainter>
#include <QLinearGradient>
#include <QPushButton>
#include "vigradientcreator.h"

class ViPushButton : public QPushButton
{
	Q_OBJECT

	public:

		ViPushButton(QWidget *parent = 0);

		void setSize(int width, int height);
		void setIcon(ViThemeImage image, ViThemeImage::State state);
		void setGlow(QColor color, ViGradientCreator::ViGradientType type = ViGradientCreator::Rectangle, int x = -1, int y = -1, int width = -1, int height = -1);
		void setCornerRounding(qreal value);

		void setEnabled(bool enable);
		void enable();
		void disable();

	protected:

		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:

		QImage mNormalImage;
		QImage mHoveredImage;
		QImage mSelectedImage;
		QImage mDisbaledImage;

		bool mIsHover;
		bool mIsPressed;

		bool mHasGlow;
		QColor mGlowColor;
		ViGradientCreator::ViGradientType mGlowType;
		int mGlowX;
		int mGlowY;
		int mGlowWidth;
		int mGlowHeight;

		QLinearGradient mNormalGradient;
		QLinearGradient mSelectedGradient;
		QLinearGradient mDisabledGradient;
		
		qreal mCornerRounding;
};

#endif
