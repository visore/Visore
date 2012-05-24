#include "vifrequencyplotpicker.h"
#include <QMouseEvent>
#include "qwt_picker_machine.h"

ViFrequencyPlotPicker::ViFrequencyPlotPicker(ViFrequencyPlot *plot)
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

ViFrequencyPlotPicker::~ViFrequencyPlotPicker()
{
	delete mZoomer;
}

void ViFrequencyPlotPicker::setLabel(ViFrequencyPlot::Axis axis, QString label)
{
	if(axis == ViFrequencyPlot::X)
	{
		mLabelX = label;
	}
	else
	{
		mLabelY = label;
	}
}

void ViFrequencyPlotPicker::setUnit(ViFrequencyPlot::Axis axis, QString unit)
{
	if(axis == ViFrequencyPlot::X)
	{
		mUnitX = unit;
	}
	else
	{
		mUnitY = unit;
	}
}

void ViFrequencyPlotPicker::zoomToExtent()
{
	mZoomer->setZoomBase(mPlot->extent());	
	mZoomer->zoom(mPlot->extent());
}

QwtText ViFrequencyPlotPicker::trackerText(const QPoint &position) const
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
