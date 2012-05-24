#include "vispectrumplotpicker.h"
#include <QMouseEvent>
#include "qwt_picker_machine.h"

ViSpectrumPlotPicker::ViSpectrumPlotPicker(ViSpectrumPlot *plot)
	: QwtPlotPicker(plot->canvas())
{
	mPlot = plot;
	mZoomer = new QwtPlotZoomer(plot->canvas());
	mZoomer->setTrackerMode(QwtPicker::AlwaysOff);

	setStateMachine(new QwtPickerTrackerMachine);
	setTrackerMode(QwtPicker::ActiveOnly);
	setRubberBand(QwtPicker::VLineRubberBand);
	setRubberBandPen(QPen(Qt::green));
}

ViSpectrumPlotPicker::~ViSpectrumPlotPicker()
{
	delete mZoomer;
}

void ViSpectrumPlotPicker::setLabel(ViSpectrumPlot::Axis axis, QString label)
{
	if(axis == ViSpectrumPlot::X)
	{
		mLabelX = label;
	}
	else
	{
		mLabelY = label;
	}
}

void ViSpectrumPlotPicker::setUnit(ViSpectrumPlot::Axis axis, QString unit)
{
	if(axis == ViSpectrumPlot::X)
	{
		mUnitX = unit;
	}
	else
	{
		mUnitY = unit;
	}
}

void ViSpectrumPlotPicker::zoomToExtent()
{
	mZoomer->setZoomBase(mPlot->extent());	
	mZoomer->zoom(mPlot->extent());
}

QwtText ViSpectrumPlotPicker::trackerText(const QPoint &position) const
{
	QColor bg(Qt::white);
	bg.setAlpha(200);
	QPointF point = invTransform(position);

	QString valueX = QString::number(point.x(), 'f', 4);
	QString valueY = QString::number(mPlot->valueAt(point.x()), 'f', 4);
	qint32 stringSizeX = qMax(mLabelX.size(), mLabelY.size()) + 2;
	qint32 stringSizeY = qMax(valueX.size(), valueY.size());

	QString string;
	QTextStream stream(&string);
	stream << qSetFieldWidth(stringSizeX) << left << (mLabelX + ": ") << qSetFieldWidth(stringSizeY) << left << valueX << qSetFieldWidth(0) << left << mUnitX << endl;
	stream << qSetFieldWidth(stringSizeX) << left << (mLabelY + ": ") << qSetFieldWidth(stringSizeY) << right << valueY << qSetFieldWidth(0) << mUnitY;

	QwtText text(string);
	text.setFont(QFont("Courier"));
	text.setBackgroundBrush( QBrush( bg ));

	return text;
}
