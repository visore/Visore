#ifndef VIFREQUENCYPLOTPICKER_H
#define VIFREQUENCYPLOTPICKER_H

#include "vifrequencyplot.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"

class ViFrequencyPlotPicker : public QwtPlotPicker
{

	public:

		ViFrequencyPlotPicker(ViFrequencyPlot *plot);
		~ViFrequencyPlotPicker();

		void setLabel(ViFrequencyPlot::Axis axis, QString label);
		void setUnit(ViFrequencyPlot::Axis axis, QString unit);

		void zoomToExtent();
		QwtText trackerText(const QPoint &position) const;

	private:

		QString mLabelX;
		QString mLabelY;
		QString mUnitX;
		QString mUnitY;

		QwtPlotZoomer *mZoomer;
		ViFrequencyPlot *mPlot;

};

#endif
