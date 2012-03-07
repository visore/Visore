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

		void setIcons(QImage normalImage, QImage selectedImage);
		void setIcons(QImage normalImage, QImage hoverImage, QImage selectedImage);
		void setNormalIcon(QImage image);
		void setHoverIcon(QImage image);
		void setSelectedIcon(QImage image);

		void setGlow(QColor color, ViGradientCreator::ViGradientType type = ViGradientCreator::Rectangle, int x = -1, int y = -1, int width = -1, int height = -1);

	protected:
		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		QImage mNormalImage;
		QImage mHoverImage;
		QImage mSelectedImage;

		bool mIsHover;
		bool mIsCheckable;
		bool mIsChecked;
		bool mIsPressed;

		bool mHasGlow;
		QColor mGlowColor;
		ViGradientCreator::ViGradientType mGlowType;
		int mGlowX;
		int mGlowY;
		int mGlowWidth;
		int mGlowHeight;
};

#endif
