#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QThread>
#include "viwidget.h"
#include "viobject.h"
#include "viwaveformtile.h"

class ViWaveFormWidget;

class ViWaveFormWidgetThread : public QThread
{
	Q_OBJECT

	signals:
		void tileAvailable();

	private slots:
		void changed(QList<double> list);

	public:
		ViWaveFormWidgetThread(ViWaveFormWidget *widget);
		~ViWaveFormWidgetThread();
		void run();

	public:
		ViWaveFormWidget *mWidget;
		QList<ViWaveFormTile*> mTiles;
		QList<QList<double> > mLists;
};

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void receiveTile();

	public:
		ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent = 0);
		~ViWaveFormWidget();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		ViWaveFormWidgetThread *mThread;
		
};

#endif
