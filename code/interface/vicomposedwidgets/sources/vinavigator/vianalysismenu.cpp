#include "vianalysismenu.h"
#include "vinavigator.h"
#include "ui_vianalysismenu.h"

ViAnalysisMenu::ViAnalysisMenu(ViNavigator *navigator)
	: ViWidget(navigator)
{
	mUi = new Ui::ViAnalysisMenu();
	mUi->setupUi(this);
	mNavigator = navigator;

	mSpectrumIndex = mNavigator->addStackWidget(&mSpectrumWidget);
	QObject::connect(mUi->spectrumButton, SIGNAL(clicked()), this, SLOT(showSpectrum()));

	mMatchIndex = mNavigator->addStackWidget(&mMatchWidget);
	QObject::connect(mUi->matchButton, SIGNAL(clicked()), this, SLOT(showMatch()));
}

ViAnalysisMenu::~ViAnalysisMenu()
{
	delete mUi;
}

void ViAnalysisMenu::showSpectrum()
{
	mNavigator->setStackIndex(mSpectrumIndex);
}

void ViAnalysisMenu::showMatch()
{
	mNavigator->setStackIndex(mMatchIndex);
}
