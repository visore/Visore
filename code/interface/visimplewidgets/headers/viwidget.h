#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include "viaudioengine.h"
#include "viid.h"

class ViWidget : public QWidget, public ViId
{

	public:

		ViWidget(QWidget *parent = 0);
		ViAudioEngine* engine();
		virtual void refresh();
		//virtual void setStyleSheet(QString styleSheet);
		//virtual void addStyleSheet(QString styleSheet);
		//virtual QString styleSheet();

		virtual void setSize(int width, int height);
		virtual void setBorder(QColor color = Qt::gray, int thickness = 1, int roundness = 0);

	protected:

		virtual void paintEvent(QPaintEvent *event);

	protected:

		QWidget *mParent;
		ViAudioEngine *mEngine;
		QString mStyleSheet;

QString m;

};

#endif
