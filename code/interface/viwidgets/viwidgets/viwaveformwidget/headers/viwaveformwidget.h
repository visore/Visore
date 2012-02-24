#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#define COMPRESSION_LEVEL_1 1
#define COMPRESSION_LEVEL_2 10
#define COMPRESSION_LEVEL_3 100
#define COMPRESSION_LEVEL_4 500
#define COMPRESSION_LEVEL_5 1000
#define COMPRESSION_LEVEL_6 2500
#define COMPRESSION_LEVEL_7 5000
#define COMPRESSION_LEVEL_8 7500
#define COMPRESSION_LEVEL_9 10000
#define COMPRESSION_LEVEL_10 15000
#define COMPRESSION_LEVEL_11 20000
#define COMPRESSION_LEVEL_12 30000
#define COMPRESSION_LEVEL_13 40000
#define COMPRESSION_LEVEL_14 50000
#define COMPRESSION_LEVEL_15 70000
#define COMPRESSION_LEVEL_16 100000

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
#include <cfloat>
#include "viwidget.h"
#include "viobject.h"
#include "vithememanager.h"
#include "viwaveform.h"

class ViWaveFormWidget;

class ViWaveFormWidgetThread : public QThread
{
	Q_OBJECT

	signals:
		void tileAvailable();

	public slots:
		void positionChanged(ViAudioPosition position);

	private slots:
		void changed(ViWaveFormChunk *chunk);

	public:
		ViWaveFormWidgetThread(ViWaveFormWidget *widget);
		void run();
		void changeSize();
		void setCompression(qint32 compression);

	public:
		ViWaveFormWidget *mWidget;
		QList<ViWaveFormChunk*> mChunks;
		ViWaveFormChunk *mRemains;
		qint64 mPosition;
		ViWaveForm mForm;
		QMutex mMutex;
};

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	public:
		ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent = 0);
		~ViWaveFormWidget();
		void analyze();
		void zoomIn();
		void zoomOut();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		ViWaveFormWidgetThread *mThread;
		QWidget *mParent;
		QList<qint32> mCompressionLevels;
		qint8 mCurrentCompressionLevel;
		qint32 mOldPosition;
};

#endif
