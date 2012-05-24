#include "vispectrumplotmagnifier.h"
#include <QMouseEvent>

ViSpectrumPlotMagnifier::ViSpectrumPlotMagnifier(ViSpectrumPlot *plot)
	: QwtPlotMagnifier(plot->canvas())
{
	mPlot = plot;
	mZoomer = new QwtPlotZoomer(plot->canvas());
	setAxisEnabled (0, false);
	setAxisEnabled (1, false);
}

ViSpectrumPlotMagnifier::~ViSpectrumPlotMagnifier()
{
	delete mZoomer;
}

void ViSpectrumPlotMagnifier::zoomToExtent()
{
	mZoomer->zoom(mPlot->extent());
}
/*
void ViSpectrumPlotMagnifier::widgetMouseReleaseEvent(QMouseEvent *event)
{
	QwtPlotMagnifier::widgetMouseReleaseEvent(event);
	if(event->button() == Qt::RightButton)
	{
		zoomToExtent();
	}
}*/
