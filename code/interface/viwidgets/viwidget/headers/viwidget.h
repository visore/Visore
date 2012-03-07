#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include "viaudioengine.h"

class ViWidget : public QWidget
{
	public:
		ViWidget(QWidget *parent = 0);
		virtual void setEngine(ViAudioEngine *engine);
		ViAudioEngine* engine();
	
	public:
		virtual void refresh();

	protected:
		QWidget *mParent;
		ViAudioEngine *mEngine;
};

#endif
