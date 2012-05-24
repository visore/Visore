#include "vianalysiswidget.h"
#include "ui_vianalysiswidget.h"

ViAnalysisWidget::ViAnalysisWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViAnalysisWidget();
	mUi->setupUi(this);

	QObject::connect(mUi->spectrumButton, SIGNAL(clicked()), this, SLOT(openSpectrum()));
}

ViAnalysisWidget::~ViAnalysisWidget()
{
	delete mUi;
}

void ViAnalysisWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	mSpectrum.setEngine(engine);
}

void ViAnalysisWidget::openSpectrum()
{
	mSpectrum.replot();
	mSpectrum.show();
}
