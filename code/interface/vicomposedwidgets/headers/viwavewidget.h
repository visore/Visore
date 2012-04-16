#ifndef VIWAVEWIDGET_H
#define VIWAVEWIDGET_H

#include <QMouseEvent>
#include <QPainter>
#include "viwidget.h"
#include "vithememanager.h"

class ViWaveWidget : public ViWidget
{
	Q_OBJECT

	signals:
		void pointerMoved(qint32 position);

	private slots:
		void positionChanged(ViAudioPosition position);

	public:
		ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		void setZoomLevel(qint16 level);
		qint64 position();
		qreal maximum(qint64 position);
		qreal minimum(qint64 position);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		ViWaveForm *mForm;
		qint64 mPosition;
		qint16 mZoomLevel;

		qint32 mHalfWidth;
		qint32 mHalfHeight;
		qreal mRatio;
		bool mUnderCutOff;
};

#endif
