#include "vispectrumanalysiswidget.h"
#include "ui_vispectrumanalysiswidget.h"
#include "vimainwindow.h"

#define EXTRA_SPACE 0.05

ViSpectrumAnalysisWidget::ViSpectrumAnalysisWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViSpectrumAnalysisWidget();
	mUi->setupUi(this);

	/*mWasInitialized = false;

	QObject::connect(mUi->sizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(recalculate()));
	QObject::connect(mUi->windowBox, SIGNAL(currentIndexChanged(int)), this, SLOT(recalculate()));

	QObject::connect(mUi->frequencyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->valueBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->notationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));

	QObject::connect(mEngine, SIGNAL(spectrumFinished()), this, SLOT(replot()));
	QObject::connect(mEngine, SIGNAL(spectrumProgressed(short)), ViMainWindow::instance(), SLOT(progress(short)));*/
}

ViSpectrumAnalysisWidget::~ViSpectrumAnalysisWidget()
{
	//delete mUi;
}

void ViSpectrumAnalysisWidget::showEvent(QShowEvent *event)
{
	if(!mWasInitialized)
	{
		mWasInitialized = true;
		recalculate();
	}
	ViWidget::showEvent(event);
}

void ViSpectrumAnalysisWidget::recalculate()
{
	//ViMainWindow::instance()->showLoading(true, false, ViLoadingWidget::Text, "Calculating Spectrum");
    engine()->calculateSpectrum(mUi->sizeBox->currentText().toInt(), mUi->windowBox->currentText());
}

void ViSpectrumAnalysisWidget::replot()
{
	/*ViRealSpectrum plot = mEngine->spectrum();
	qint32 size = plot.size();

	if(size > 0)
	{
		QVector<qreal> x(size);
		QVector<qreal> y(size);

		QString labelX = "Frequency";
		QString labelY = "Amplitude";
		QString unitX = "";
		QString unitY = "";

		bool fill = false;
		bool drawImaginary = true;

		qreal xMaximum, yMinimum, yMaximum;
		if(mUi->frequencyBox->currentIndex() == 0)
		{
			xMaximum = plot[size - 1].frequencyRange();
			for(int i = 0; i < size; ++i)
			{
				x[i] = plot[i].frequencyRange();
			}
			unitX = "";
		}
		else
		{
			xMaximum = plot[size - 1].frequencyHertz();
			for(int i = 0; i < size; ++i)
			{
				x[i] = plot[i].frequencyHertz();
			}
			unitX = "Hz";
		}

		if(mUi->notationBox->currentIndex() == 0)
		{
			fill = true;
			if(mUi->valueBox->currentIndex() == 0)
			{
				yMinimum = plot.minimum().polar().amplitude().real() * (1 + EXTRA_SPACE);
				yMaximum = plot.maximum().polar().amplitude().real() * (1 + EXTRA_SPACE);
				for(int i = 0; i < size; ++i)
				{
					y[i] = plot[i].polar().amplitude().real();
				}
				unitY = "";
			}
			else
			{
			
				yMinimum = plot.minimum().polar().decibel().real() * (1 + EXTRA_SPACE);
				yMaximum = plot.maximum().polar().decibel().real() * (1 + EXTRA_SPACE);
				for(int i = 0; i < size; ++i)
				{
					y[i] = plot[i].polar().decibel().real();
				}
				unitY = "dB";
			}
		}
		else
		{
			if(mUi->valueBox->currentIndex() == 0)
			{
				yMinimum = plot.minimum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
				yMaximum = plot.maximum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
				for(int i = 0; i < size; ++i)
				{
					y[i] = plot[i].rectangular().amplitude().real();
				}
				unitY = "";
			}
			else
			{
				yMinimum = plot.minimum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
				yMaximum = plot.maximum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
				for(int i = 0; i < size; ++i)
				{
					y[i] = plot[i].rectangular().decibel().real();
				}
				unitY = "dB";
			}
		}

		mUi->plot->setScale(ViSpectrumPlot::X, 0, xMaximum);
		mUi->plot->setScale(ViSpectrumPlot::Y, yMinimum, yMaximum);
		mUi->plot->setLabel(ViSpectrumPlot::X, labelX);
		mUi->plot->setLabel(ViSpectrumPlot::Y, labelY);
		mUi->plot->setUnit(ViSpectrumPlot::X, unitX);
		mUi->plot->setUnit(ViSpectrumPlot::Y, unitY);
		mUi->plot->fill(fill);
		mUi->plot->setData(x, y);

	}
	ViMainWindow::instance()->hideLoading();*/
}


