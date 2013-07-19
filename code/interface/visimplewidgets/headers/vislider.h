#ifndef VISLIDER_H
#define VISLIDER_H

#include <QSlider>
#include "vithememanager.h"

class ViSlider : public QSlider
{

	Q_OBJECT

	signals:

		void pressed();
		void released();

	public:

		ViSlider(QWidget *parent = NULL);

	protected:

		void mousePressEvent(QMouseEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);

	private:

		void changePosition(int pixel);

	private:

		bool mIsPressed;

};

#endif
