#include "vianalysiswidget.h"
#include "ui_vianalysiswidget.h"
#include "vimainwindow.h"

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

void ViAnalysisWidget::openSpectrum()
{
	mSpectrum.recalculate();
	ViMainWindow::instance()->showSpectrum();
}
