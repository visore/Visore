#ifndef VIWAVEOVERLAYWIDGET_H
#define VIWAVEOVERLAYWIDGET_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <viwidget.h>
#include <viwaveform.h>

class ViWaveOverlayWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void pointerChanged(qint32 position);
		void pointerValuesChanged(qreal maximum, qreal minimum, qreal average);
		void zoomLevelChanged(qint16 levels);
		void positionChanged(ViAudioPosition position);

		void mouseMoved();

	public slots:

		void setPosition(ViAudioPosition position);
		void setPosition(qint64 sample);
		void setPointer(qint32 position);

	private slots:

		void updateWave();

	public:

		ViWaveOverlayWidget(ViWidget *parent = 0);
		~ViWaveOverlayWidget();

		void setZoomLevel(qint16 level);
		int zoomLevel();

		void setWaveForm(ViWaveForm *form, ViAudioFormat format = ViAudioFormat());
		void clear();

	protected:

		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void calculatePosition();

	private:

		ViWidget *mPastBox;

		ViWaveForm *mForm;
		ViAudioFormat mFormat;
		qint64 mPosition;
		qint16 mZoomLevel;

		qint32 mPointerPositionPixel;
		qint32 mPointerPositionSample;

		qint32 mHalfWidth;
		qint32 mHalfHeight;

		qreal mZoomRatio;
		qreal mHeightRatio;
		bool mUnderCutOff;

		bool mScroll;
		int mScrollStart;

};

#endif
