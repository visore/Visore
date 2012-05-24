#ifndef VIWAVEOVERLAYWIDGET_H
#define VIWAVEOVERLAYWIDGET_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include "viwidget.h"
#include "vithememanager.h"

class ViWaveOverlayWidget : public ViWidget
{
	Q_OBJECT

	signals:
		void pointerMoved(qint32 position);
		void pointerValuesChanged(qreal maximum, qreal minimum, qreal maximumAverage, qreal minimumAverage);
		void zoomLevelChanged(qint16 levels);

	public slots:
		void setPointer(qint32 position);

	private slots:
		void positionChanged(ViAudioPosition position);

	public:
		ViWaveOverlayWidget(ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		void setZoomLevel(qint16 level);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent *event);
		void calculatePosition();

	private:
		ViWaveForm *mForm;
		qint64 mPosition;
		qint16 mZoomLevel;

		qint32 mPointerPositionPixel;
		qint32 mPointerPositionSample;

		qint32 mHalfWidth;
		qint32 mHalfHeight;

		qreal mZoomRatio;
		qreal mHeightRatio;
		bool mUnderCutOff;
};

#endif
