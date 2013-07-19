#ifndef VIVOLUMEBAR_H
#define VIVOLUMEBAR_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vigradientcreator.h>

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

	public:

		ViVolumeBar(QWidget *parent = 0, ViVolumeBar::ViVolumeBarType type = ViVolumeBar::Angled);
		void setType(ViVolumeBar::ViVolumeBarType type);
		void setValue(qint8 value);
		qint8 value();
		void setSize(qint16 width, qint16 height);

	protected:

		void paintEvent(QPaintEvent*);
		void enterEvent(QEvent*);
		void leaveEvent(QEvent*);
		void mousePressEvent(QMouseEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
		void setXValue(qint16 value);

	private:

		bool mIsHover;
		bool mIsPressed;
		qint16 mValue;
		qint16 mRealValue;
		ViVolumeBar::ViVolumeBarType mType;

};

#endif
