#ifndef VISPECTRUMWIDGET_H
#define VISPECTRUMWIDGET_H

#include "vithememanager.h"
#include "viaudioposition.h"
#include "viwidget.h"

class ViSpectrumWidget : public ViWidget
{
	Q_OBJECT

	public:

		enum Mode
		{
			Amplitude,
			Decibel
		};

	public slots:

		void addSpectrum(ViRealSpectrum spectrum, qint64 milliseconds);
		void update(ViAudioPosition position);

	public:

		ViSpectrumWidget(QWidget *parent = 0);
		void setBars(int bars);
		void setMode(ViSpectrumWidget::Mode mode);
		void setSpacing(int spacing);

	protected:

		void resizeEvent(QResizeEvent *event);
		void paintEvent(QPaintEvent *event);
		
		QVector<qbyte16u> summarizeSpectrum(QVector<qbyte16u> &spectrum);
		void addAmplitudeData(ViRealSpectrum &spectrum);
		void addDecibelData(ViRealSpectrum &spectrum);

	private:

		void (ViSpectrumWidget::*addSpectrumData)(ViRealSpectrum&);

		QList<QVector<qbyte16u> > mSpectrums;
		QList<qint64> mIntervals;
		QVector<qbyte16u> mCurrentSpectrum;
		int mCurrentSpectrumIndex;
		qint64 mCurrentTime;

		int mBars;
		int mOriginalBars;
		int mSpacing;

		QBrush mMainBrush;
		QBrush mMirrorBrush;
		int mMainHeight;

};

#endif
