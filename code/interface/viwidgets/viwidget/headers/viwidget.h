#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include "viaudioengine.h"

class ViWidget : public QWidget
{
	public:
		ViWidget(ViAudioEngine *engine, QWidget *parent = 0);
	 
	public:
		ViAudioEngine *mEngine;
		virtual void refresh();

	protected:
		QWidget *mParent;
};

#endif
