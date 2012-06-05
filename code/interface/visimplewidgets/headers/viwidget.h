#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include "viaudioengine.h"

class ViWidget : public QWidget
{

	public:

		ViWidget(QWidget *parent = 0);
		ViAudioEngine* engine();
		virtual void refresh();

	protected:

		virtual void paintEvent(QPaintEvent *event);

	protected:

		QWidget *mParent;
		ViAudioEngine *mEngine;

};

#endif
