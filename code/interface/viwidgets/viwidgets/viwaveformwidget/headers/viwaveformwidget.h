#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#define COMPRESSION_LEVEL_1 1
#define COMPRESSION_LEVEL_2 1000
#define COMPRESSION_LEVEL_3 5000
#define COMPRESSION_LEVEL_4 10000
#define COMPRESSION_LEVEL_5 50000

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
		void positionChanged(qint64 bytes, qint64 milliseconds, qint8 bits);

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
