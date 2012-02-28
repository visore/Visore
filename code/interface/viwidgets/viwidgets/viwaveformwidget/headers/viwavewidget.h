#ifndef VIWAVEWIDGET_H
#define VIWAVEWIDGET_H

#define COMPRESSIONS 14
#define COMPRESSION_LEVEL_14 100
#define COMPRESSION_LEVEL_13 500
#define COMPRESSION_LEVEL_12 1000
#define COMPRESSION_LEVEL_11 2500
#define COMPRESSION_LEVEL_10 5000
#define COMPRESSION_LEVEL_9 7500
#define COMPRESSION_LEVEL_8 10000
#define COMPRESSION_LEVEL_7 15000
#define COMPRESSION_LEVEL_6 20000
#define COMPRESSION_LEVEL_5 30000
#define COMPRESSION_LEVEL_4 40000
#define COMPRESSION_LEVEL_3 50000
#define COMPRESSION_LEVEL_2 70000
#define COMPRESSION_LEVEL_1 100000

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
#include <cfloat>
#include "viwidget.h"
#include "viobject.h"
#include "vithememanager.h"
#include "viwaveform.h"
#include "viwidgettoolbar.h"

class ViWaveWidget;

class ViWaveWidgetThread : public QThread
{
	Q_OBJECT

	friend class ViWaveWidget;

	signals:
		void tileAvailable();

	public slots:
		void positionChanged(ViAudioPosition position);

	private slots:
		void changed(ViWaveFormChunk *chunk);
		void analyze(int size);

	public:
		ViWaveWidgetThread(ViWaveWidget *widget);
		void run();
		

	private:
		QList<qint32> mCompressionLevels;
		ViWaveWidget *mWidget;
		QList<ViWaveFormChunk*> mChunks;
		qint64 mPosition;
		QList<ViWaveForm> mForms;
		QMutex mMutex;
		QMutex mFormMutex;
		ViAudioBuffer::ViAudioBufferType mBufferType;
};

class ViWaveWidget : public ViWidget
{
	Q_OBJECT

	public slots:
		void zoomIn();
		void zoomOut();

	public:
		ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		~ViWaveWidget();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:
		ViWidgetToolbar *mToolbar;
		ViWaveWidgetThread *mThread;
		qint8 mCurrentCompressionLevel;
};

#endif
