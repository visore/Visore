#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
#include <cfloat>
#include "viwidget.h"
#include "viobject.h"
#include "vithememanager.h"
#include "viwavecollection.h"

class ViWaveFormWidget;

class ViWaveFormWidgetThread : public QThread
{
	Q_OBJECT

	signals:
		void tileAvailable();

	private slots:
		void changed(ViWaveFormChunk *chunk);
		void positionChanged(ViAudioPosition position);

	public:
		ViWaveFormWidgetThread(ViWaveFormWidget *widget);
		void run();

	public:
		ViWaveFormWidget *mWidget;
		QList<ViWaveFormChunk*> mChunks;
		ViWaveFormChunk *mRemains;
		qint64 mPosition;
		ViWaveCollection mCollection;
		QMutex mMutex;
};

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	public:
		ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent = 0);
		~ViWaveFormWidget();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		ViWaveFormWidgetThread *mThread;
		QWidget *mParent;
};

#endif
