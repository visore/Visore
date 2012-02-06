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
		void positionChanged(qint64 bytes, qint64 milliseconds);

	public:
		ViWaveFormWidgetThread(ViWaveFormWidget *widget);
		~ViWaveFormWidgetThread();
		void run();

	public:
		ViWaveFormWidget *mWidget;
		QList<ViWaveFormTile*> mTiles;
		QList<QList<double> > mLists;
		qint64 mPosition;
};

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	public:
		enum ViWaveForm
		{
			Combined = 0,
			Seperated = 1
		};
		ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent = 0);
		~ViWaveFormWidget();
		ViWaveFormWidget::ViWaveForm waveForm();
		void setWaveForm(ViWaveFormWidget::ViWaveForm form);

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		ViWaveFormWidgetThread *mThread;
		ViWaveFormWidget::ViWaveForm mWaveForm;
		int mCurrentX;
		QPainter *mPainter;
};

#endif
