#ifndef VIWAVEWIDGET_H
#define VIWAVEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
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
		void changed(QSharedPointer<ViWaveFormChunk> chunk);
		void analyze(int size);

	public:
		ViWaveWidgetThread(ViWaveWidget *widget);
		void run();
		
	private:
		ViWaveWidget *mWidget;
		QList<QSharedPointer<ViWaveFormChunk> > mChunks;
		qint64 mPosition;
		ViWaveForm mForm;
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
		qint8 mZoomLevel;
};

#endif
