#ifndef VIFREQUENCYPLOT_H
#define VIFREQUENCYPLOT_H

#include "vithememanager.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

class ViFrequencyPlotPicker;
class ViFrequencyPlotMagnifier;

class ViFrequencyPlot : public QwtPlot
{

	public:

		enum Axis
		{
			X = 0,
			Y = 1
		};

		ViFrequencyPlot(QWidget *parent = 0);
		~ViFrequencyPlot();

		void setData(QVector<qreal> dataX, QVector<qreal> dataY);
		void setScale(ViFrequencyPlot::Axis axis, qreal minimum, qreal maximum);
		void setLabel(ViFrequencyPlot::Axis axis, QString label);
		void setUnit(ViFrequencyPlot::Axis axis, QString unit);
		void fill(bool fill);

		QRectF extent();
		qreal valueAt(qreal x);

	private:

		void clear();

	private:

		qreal *mDataX;
		qreal *mDataY;
		qint32 mDataCount;
		qreal mLength;

		QString mLabelX;
		QString mLabelY;
		QString mUnitX;
		QString mUnitY;

		//ViFrequencyPlotMagnifier *mMagnifier;
		ViFrequencyPlotPicker *mPicker;
		QwtPointSeriesData mData;
		QwtPlotCurve mCurve;
		QwtPlotGrid mGrid;
		

};

#endif
