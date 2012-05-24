#ifndef VIWAVEBASEWIDGET_H
#define VIWAVEBASEWIDGET_H

#include <QPainter>
#include "viwidget.h"
#include "vithememanager.h"

class ViWaveBaseWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void positionChanged(ViAudioPosition position);

	public:
		ViWaveBaseWidget(ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		void setZoomLevel(qint16 level);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

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
