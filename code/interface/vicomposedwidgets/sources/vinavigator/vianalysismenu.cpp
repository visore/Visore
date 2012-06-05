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
	mMatchIndex = addStackWidget(&mMatchWidget);
	QObject::connect(mUi->matchButton, SIGNAL(clicked()), this, SLOT(showMatch()));
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

void ViAnalysisMenu::showMatch()
{
	setCurrentStackIndex(mMatchIndex);
	setStackIndex(mMatchIndex);
}
