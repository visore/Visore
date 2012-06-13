#ifndef VISPECTRUMPLOT_H
#define VISPECTRUMPLOT_H

#include "vithememanager.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

class ViSpectrumPlotPicker;

class ViSpectrumPlot : public QwtPlot
{

	public:

		enum Axis
		{
			X = 0,
			Y = 1
		};

		ViSpectrumPlot(QWidget *parent = 0);
		~ViSpectrumPlot();

		void setData(QVector<qreal> dataX, QVector<qreal> dataY);
		void setScale(ViSpectrumPlot::Axis axis, qreal minimum, qreal maximum);
		void setLabel(ViSpectrumPlot::Axis axis, QString label);
		void setUnit(ViSpectrumPlot::Axis axis, QString unit);
		void fill(bool fill);

		QRectF extent();
		qreal valueAt(qreal x);

	private:

		void clear();

	private:

		/*qreal *mDataX;
		qreal *mDataY;
		qint32 mDataCount;
		qreal mLength;

		QString mLabelX;
		QString mLabelY;
		QString mUnitX;
		QString mUnitY;*/

		//ViSpectrumPlotPicker *mPicker;
		//QwtPointSeriesData mData;
		//QwtPlotCurve mCurve;
		//QwtPlotGrid mGrid;
		

};

#endif
