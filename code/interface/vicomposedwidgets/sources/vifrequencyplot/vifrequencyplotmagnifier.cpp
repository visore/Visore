#include "vifrequencyplotmagnifier.h"
#include <QMouseEvent>

ViFrequencyPlotMagnifier::ViFrequencyPlotMagnifier(ViFrequencyPlot *plot)
	: QwtPlotMagnifier(plot->canvas())
{
	mPlot = plot;
	mZoomer = new QwtPlotZoomer(plot->canvas());
}

ViFrequencyPlotMagnifier::~ViFrequencyPlotMagnifier()
{
	delete mZoomer;
}

void ViFrequencyPlotMagnifier::zoomToExtent()
{
	mZoomer->zoom(mPlot->extent());
}

void ViFrequencyPlotMagnifier::widgetMouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton)
	{
		//zoomToExtent();
	}
	else
	{
		//QwtPlotMagnifier::widgetMouseReleaseEvent(event);
	}
}
