#ifndef VIWAVEWIDGET_H
#define VIWAVEWIDGET_H

#include <QWidget>
#include <QPainter>
#include "viwidget.h"
#include "vithememanager.h"
#include "viwidgettoolbar.h"

class ViWaveWidget : public ViWidget
{
	Q_OBJECT

	public slots:
		void zoomIn();
		void zoomOut();

	private slots:
		void positionChanged(ViAudioPosition position);

	public:
		ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		~ViWaveWidget();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:
		ViWaveForm *mForm;
		ViWidgetToolbar *mToolbar;
		qint16 mZoomLevel;
		qint64 mPosition;
};

#endif
