#ifndef VIFREQUENCYPLOTMAGNIFIER_H
#define VIFREQUENCYPLOTMAGNIFIER_H

#include "vifrequencyplot.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_zoomer.h"

class ViFrequencyPlotMagnifier : public QwtPlotMagnifier
{

	public:

		ViFrequencyPlotMagnifier(ViFrequencyPlot *plot);
		~ViFrequencyPlotMagnifier();
		void zoomToExtent();

	protected:

		void widgetMouseReleaseEvent(QMouseEvent *event);

	private:

		QwtPlotZoomer *mZoomer;
		ViFrequencyPlot *mPlot;

};

#endif
