#include "vianalysismenu.h"
#include "ui_vianalysismenu.h"

ViAnalysisMenu::ViAnalysisMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViAnalysisMenu();
	mUi->setupUi(this);

	//Spectrum analysis widget
	mSpectrumIndex = addStackWidget(&mSpectrumWidget);
	QObject::connect(mUi->spectrumButton, SIGNAL(clicked()), this, SLOT(showSpectrum()));

	//Signal matching widget
	mCorrelationIndex = addStackWidget(&mCorrelationWidget);
	QObject::connect(mUi->matchButton, SIGNAL(clicked()), this, SLOT(showCorrelation()));
}

ViAnalysisMenu::~ViAnalysisMenu()
{
	delete mUi;
}

void ViAnalysisMenu::showSpectrum()
{
	setCurrentStackIndex(mSpectrumIndex);
	setStackIndex(mSpectrumIndex);
}

void ViAnalysisMenu::showCorrelation()
{
	setCurrentStackIndex(mCorrelationIndex);
	setStackIndex(mCorrelationIndex);
}
