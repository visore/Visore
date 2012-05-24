#ifndef VITABBUTTON_H
#define VITABBUTTON_H

#define TAB_WIDTH 100
#define TAB_HEIGHT 35
#define TAB_OFFSET 5

#include <QPainter>
#include <QWidget>
#include "vithememanager.h"
#include "vigradientcreator.h"

class ViTabButton : public QWidget
{
	Q_OBJECT

	signals:
		void selected(qint8 id);

	public:
		ViTabButton(QString title, qint8 id, QWidget *parent = 0);
		void select(bool select);
		void setSize(int width, int height);
		void setRounding(qint8 angle);
		qint8 id();

	protected:
		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		QString mTitle;
		qint8 mId;
		qint8 mAngle;
		bool mIsHover;
		bool mIsSelected;
};

#endif
