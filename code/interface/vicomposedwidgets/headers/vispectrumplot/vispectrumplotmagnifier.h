#ifndef VISPECTRUMPLOTMAGNIFIER_H
#define VISPECTRUMPLOTMAGNIFIER_H

#include "vispectrumplot.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_zoomer.h"

class ViSpectrumPlotMagnifier : public QwtPlotMagnifier
{

	public:

		ViSpectrumPlotMagnifier(ViSpectrumPlot *plot);
		~ViSpectrumPlotMagnifier();
		void zoomToExtent();

	protected:

		//void widgetMouseReleaseEvent(QMouseEvent *event);

	private:

		QwtPlotZoomer *mZoomer;
		ViSpectrumPlot *mPlot;

};

#endif
