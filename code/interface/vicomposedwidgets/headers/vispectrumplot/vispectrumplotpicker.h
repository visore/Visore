#ifndef VISPECTRUMPLOTPICKER_H
#define VISPECTRUMPLOTPICKER_H

#include "vispectrumplot.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"

class ViSpectrumPlotPicker : public QwtPlotPicker
{

	public:

		ViSpectrumPlotPicker(ViSpectrumPlot *plot);
		~ViSpectrumPlotPicker();

		void setLabel(ViSpectrumPlot::Axis axis, QString label);
		void setUnit(ViSpectrumPlot::Axis axis, QString unit);

		void zoomToExtent();
		QwtText trackerText(const QPoint &position) const;

	private:

		QString mLabelX;
		QString mLabelY;
		QString mUnitX;
		QString mUnitY;

		QwtPlotZoomer *mZoomer;
		ViSpectrumPlot *mPlot;

};

#endif
