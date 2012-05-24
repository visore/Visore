#include "vifrequencyplotmagnifier.h"
#include <QMouseEvent>

ViFrequencyPlotMagnifier::ViFrequencyPlotMagnifier(ViFrequencyPlot *plot)
	: QwtPlotMagnifier(plot->canvas())
{
	mPlot = plot;
	mZoomer = new QwtPlotZoomer(plot->canvas());
	setAxisEnabled (0, false);
	setAxisEnabled (1, false);
}

ViFrequencyPlotMagnifier::~ViFrequencyPlotMagnifier()
{
	delete mZoomer;
}

void ViFrequencyPlotMagnifier::zoomToExtent()
{
	mZoomer->zoom(mPlot->extent());
}
/*
void ViFrequencyPlotMagnifier::widgetMouseReleaseEvent(QMouseEvent *event)
{
	QwtPlotMagnifier::widgetMouseReleaseEvent(event);
	if(event->button() == Qt::RightButton)
	{
		zoomToExtent();
	}
}*/
