#ifndef VISPECTRUMWIDGET_H
#define VISPECTRUMWIDGET_H

#include "vithememanager.h"
#include "viaudioposition.h"
#include "viwidget.h"

class ViSpectrumWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void addSpectrum(ViRealSpectrum spectrum, qint64 milliseconds);
		void update(ViAudioPosition position);

	public:

		ViSpectrumWidget(QWidget *parent = 0);
		void setBars(int bars);

	protected:

		void paintEvent(QPaintEvent *event);
		QVector<qbyte16u> summarizeSpectrum(QVector<qbyte16u> spectrum);

	private:

		QList<QVector<qbyte16u> > mSpectrums;
		QList<qint64> mIntervals;
		QVector<qbyte16u> mCurrentSpectrum;
		int mCurrentSpectrumIndex;
		int mBars;
		bool mSpectrumChanged;
};

#endif
