#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#include <QResizeEvent>
#include "viwidgettoolbar.h"
#include "viwavewidget.h"

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	signals:
		void pointerMoved(qint32 position);

	public slots:
		void zoomIn();
		void zoomOut();
		void setPointer(qint32 position);

	public:
		ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		~ViWaveFormWidget();
		void setZoomLevel(qint16 level);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:
		ViWaveWidget *mWidget;
		ViWidgetToolbar *mToolbar;
		qint16 mZoomLevel;
		qint32 mPointerPosition;

qint32 mPointerPosition2;

		qint32 mHalfWidth;
		qint32 mHalfHeight;
		qreal mRatio;
};

#endif
