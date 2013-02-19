#ifndef VIWAVEBASEWIDGET_H
#define VIWAVEBASEWIDGET_H

#include <viwidget.h>
#include <viwaveform.h>

class ViWaveBaseWidget : public ViWidget
{

	Q_OBJECT

	public slots:

		void setPosition(ViAudioPosition position);

	private slots:

		void updateWave();

	public:

		ViWaveBaseWidget(ViWidget *parent);
		void setZoomLevel(qint16 level);
		void setWaveForm(ViWaveForm *form, ViAudioFormat format = ViAudioFormat());
		void clear();

	protected:

		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:

		ViWaveForm *mForm;
		ViAudioFormat mFormat;
		qint64 mPosition;
		qint16 mZoomLevel;

		qint32 mHalfWidth;
		qint32 mHalfHeight;
		qreal mRatio;
		bool mUnderCutOff;
		int mIntervalCounter;

};

#endif
