#ifndef VIVOLUMEBAR_H
#define VIVOLUMEBAR_H

#define PEN_WIDTH 2

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "vigradientcreator.h"

class ViVolumeBar : public QWidget
{
	Q_OBJECT

	signals:
		void valueChanged(int value);

	public:
		enum ViVolumeBarType
		{
			Straight = 0,
			Angled = 1
		};
		ViVolumeBar(QWidget *parent = 0, ViVolumeBar::ViVolumeBarType type = ViVolumeBar::Angled);
		void setType(ViVolumeBar::ViVolumeBarType type);
		void setValue(qint8 value);
		qint8 value();
		void setSize(qint16 width, qint16 height);

	protected:
		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void setXValue(qint16 value);

	private:
		bool mIsHover;
		bool mIsPressed;
		qint16 mValue;
		qint16 mRealValue;
		ViVolumeBar::ViVolumeBarType mType;
};

#endif
