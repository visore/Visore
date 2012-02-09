#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#define COMPRESSION_LEVEL_3 3500

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
#include <cfloat>
#include "viwidget.h"
#include "viobject.h"
#include "vithememanager.h"

struct ViWaveAmplitude
{
	ViWaveAmplitude(double maximum, double minimum, double averageMaximum, double averageMinimum)
	{
		mMaximum = maximum;
		mMinimum = minimum;
		mAverageMaximum = averageMaximum;
		mAverageMinimum = averageMinimum;
	}
	double mMaximum;
	double mMinimum;
	double mAverageMaximum;
	double mAverageMinimum;
};

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
		QList<ViWaveAmplitude> mAmplitudes;
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
};

#endif
