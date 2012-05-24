#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include "viaudioengine.h"

class ViWidget : public QWidget
{
	public:
		ViWidget(QWidget *parent = 0);

		ViAudioEngine* engine();

		virtual void refresh();
		virtual void setStyleSheet(QString styleSheet);
		virtual void paintEvent(QPaintEvent *event);

	protected:

		QWidget *mParent;
		ViAudioEngine *mEngine;
		QStyle *mStyle; //Qt deletes this
};

#endif
