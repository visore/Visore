#include <vispectrumanalysiswidget.h>
#include <ui_vispectrumanalysiswidget.h>
#include <vifouriertransformer.h>

#define EXTRA_SPACE 0.1

ViSpectrumAnalysisWidget::ViSpectrumAnalysisWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViSpectrumAnalysisWidget();
	mUi->setupUi(this);

	mUi->windowBox->addItems(ViFourierTransformer().windowFunctions());

	clear();

	checkNotation();

	QObject::connect(mUi->sizeBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(windowSizeChanged()));
	QObject::connect(mUi->windowBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(windowFunctionChanged()));
	QObject::connect(mUi->notationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkNotation()));
}

ViSpectrumAnalysisWidget::~ViSpectrumAnalysisWidget()
{
	clear();
	delete mUi;
}

void ViSpectrumAnalysisWidget::clear()
{
	mSpectrum = NULL;
	mUi->sizeBox->setCurrentIndex(9);
	mUi->windowBox->setCurrentIndex(0);
	mUi->notationBox->setCurrentIndex(0);
	mUi->yBox->setCurrentIndex(1);
	mUi->xBox->setCurrentIndex(1);
	mUi->plot->clear();
}

int ViSpectrumAnalysisWidget::windowSize()
{
	return mUi->sizeBox->currentText().toInt();
}

QString ViSpectrumAnalysisWidget::windowFunction()
{
	return mUi->windowBox->currentText();
}

void ViSpectrumAnalysisWidget::setSpectrum(ViRealSpectrum *spectrum)
{
	mUi->plot->clear();
	mSpectrum = spectrum;
	replot();
}

void ViSpectrumAnalysisWidget::checkNotation()
{
	QObject::disconnect(mUi->xBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::disconnect(mUi->yBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));

	mUi->yBox->clear();
	mUi->yBox->addItem("Amplitude");
	if(mUi->notationBox->currentIndex() == 0)
	{
		mUi->yBox->addItem("Decibel");
	}
	replot();

	QObject::connect(mUi->xBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
	QObject::connect(mUi->yBox, SIGNAL(currentIndexChanged(int)), this, SLOT(replot()));
}

void ViSpectrumAnalysisWidget::replot()
{
	if(mSpectrum != NULL)
	{
		qint32 size = mSpectrum->size() / 2; // Devide by 2 to remove the mirrored spectrum
		if(size > 0)
		{
			QVector<qreal> x(size);
			QVector<qreal> y(size);

			QString labelX = "Frequency";
			QString labelY = "Amplitude";
			QString unitX = "";
			QString unitY = "";

			bool fill = false;

			qreal xMaximum, yMinimum, yMaximum;
			if(mUi->xBox->currentIndex() == 0)
			{
				xMaximum = mSpectrum->at(mSpectrum->size() - 1).frequencyRange() / 2;
				for(int i = 0; i < size; ++i)
				{
					x[i] = mSpectrum->at(i).frequencyRange();
				}
				unitX = "";
			}
			else
			{
				xMaximum = mSpectrum->at(mSpectrum->size() - 1).frequencyHertz() / 2;
				for(int i = 0; i < size; ++i)
				{
					x[i] = mSpectrum->at(i).frequencyHertz();
				}
				unitX = "Hz";
			}
			if(mUi->notationBox->currentIndex() == 0)
			{
				fill = true;
				if(mUi->yBox->currentIndex() == 0)
				{
					yMinimum = mSpectrum->minimum().polar().amplitude().real() * (1 + EXTRA_SPACE);
					yMaximum = mSpectrum->maximum().polar().amplitude().real() * (1 + EXTRA_SPACE);
					for(int i = 0; i < size; ++i)
					{
						y[i] = mSpectrum->at(i).polar().amplitude().real();
					}
					unitY = "";
				}
				else
				{

					yMinimum = mSpectrum->minimum().polar().decibel().real() * (1 + EXTRA_SPACE);
					yMaximum = mSpectrum->maximum().polar().decibel().real() * (1 + EXTRA_SPACE);
					for(int i = 0; i < size; ++i)
					{
						y[i] = mSpectrum->at(i).polar().decibel().real();
					}
					unitY = "dB";
				}
			}
			else
			{
				if(mUi->yBox->currentIndex() == 0)
				{
					yMinimum = mSpectrum->minimum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
					yMaximum = mSpectrum->maximum().rectangular().amplitude().real() * (1 + EXTRA_SPACE);
					for(int i = 0; i < size; ++i)
					{
						y[i] = mSpectrum->at(i).rectangular().amplitude().real();
					}
					unitY = "";
				}
				else
				{
					yMinimum = mSpectrum->minimum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
					yMaximum = mSpectrum->maximum().rectangular().decibel().real() * (1 + EXTRA_SPACE);
					for(int i = 0; i < size; ++i)
					{
						y[i] = mSpectrum->at(i).rectangular().decibel().real();
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
			mUi->plot->zoomToExtent();
		}
	}
}


