#include "vifrequencyplot.h"
#include "vifrequencyplotmagnifier.h"
#include "vifrequencyplotpicker.h"

#include "qwt_plot_magnifier.h"
#include "qwt_legend.h"
#include "qwt_plot_canvas.h"

#include <iostream>
using namespace std;

ViFrequencyPlot::ViFrequencyPlot(QWidget *parent)
	: QwtPlot(parent)
{
	mDataX = NULL;
	mDataY = NULL;
	mDataCount = 0;
	mLength = 0;

	mLabelX = "";
	mLabelY = "";
	mUnitX = "";
	mUnitY = "";

	mCurve.setPen(QPen(Qt::red));
	mCurve.attach(this);

	mGrid.setPen(QPen(Qt::white));
	mGrid.attach(this);

	mPicker = new ViFrequencyPlotPicker(this);
	//mMagnifier = new ViFrequencyPlotMagnifier(this);

    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
    canvas()->setBorderRadius(1); // Canvas turns black on values < 1

    QPalette canvasPalette(Qt::white);
    canvasPalette.setColor(QPalette::Foreground, QColor( 133, 190, 232 ));
    canvas()->setPalette(canvasPalette);
}

ViFrequencyPlot::~ViFrequencyPlot()
{
	clear();
	//delete mMagnifier;
	delete mPicker;
}

void ViFrequencyPlot::clear()
{
	if(mDataX != NULL)
	{
		delete [] mDataX;
		mDataX = NULL;
	}
	if(mDataY != NULL)
	{
		delete [] mDataY;
		mDataY = NULL;
	}
	mDataCount = 0;
}

void ViFrequencyPlot::setData(QVector<qreal> dataX, QVector<qreal> dataY)
{
	clear();
	mDataCount = qMin(dataX.size(), dataY.size());
	mDataX = new qreal[mDataCount];
	mDataY = new qreal[mDataCount];
	for(int i = 0; i < mDataCount; ++i)
	{
		mDataX[i] = dataX[i];
		mDataY[i] = dataY[i];
	}
	mCurve.setRawSamples(mDataX, mDataY, mDataCount);
	replot();
	mPicker->zoomToExtent();
}

void ViFrequencyPlot::setScale(ViFrequencyPlot::Axis axis, qreal minimum, qreal maximum)
{
	if(axis == ViFrequencyPlot::X)
	{
		setAxisScale(xBottom, minimum, maximum);
		mLength = maximum - minimum;
	}
	else
	{
		setAxisScale(yLeft, minimum, maximum);
		mCurve.setBaseline(minimum);
	}
}

void ViFrequencyPlot::setLabel(ViFrequencyPlot::Axis axis, QString label)
{
	if(axis == ViFrequencyPlot::X)
	{
		setAxisTitle(xBottom, label);
		mLabelX = label;
	}
	else
	{
		setAxisTitle(yLeft, label);
		mLabelY = label;
	}
	mPicker->setLabel(axis, label);
}

void ViFrequencyPlot::setUnit(ViFrequencyPlot::Axis axis, QString unit)
{
	if(axis == ViFrequencyPlot::X)
	{
		setAxisTitle(xBottom, mLabelX + " (" + unit + ")");
		mUnitX = unit;
	}
	else
	{
		setAxisTitle(yLeft, mLabelY + " (" + unit + ")");
		mUnitY = unit;
	}
	mPicker->setUnit(axis, unit);
}

void ViFrequencyPlot::fill(bool fill)
{
	if(fill)
	{
		QColor color = mCurve.pen().color();
		color.setAlpha(150);
		mCurve.setBrush(color);
	}
	else
	{
		mCurve.setBrush(Qt::NoBrush);
	}
}

QRectF ViFrequencyPlot::extent()
{
	return mCurve.boundingRect();
}

qreal ViFrequencyPlot::valueAt(qreal x)
{
	if(mLength != 0)
	{
		qint32 size = mCurve.data()->size();
		qint32 index = qFloor((x * size) / mLength - 0.5);
		if(index >= 0 && index < size)
		{
			return mCurve.data()->sample(index).y();
		}
	}
	return 0;
}
