#include "vianalysismenu.h"
#include "ui_vianalysismenu.h"

ViAnalysisMenu::ViAnalysisMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViAnalysisMenu();
	mUi->setupUi(this);

	//Spectrum
	/*mUi->spectrumButton->setIcon(ViThemeManager::image("spectrum.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->spectrumButton->setCheckable(true);
	mSpectrumIndex = addStackWidget(&mSpectrumWidget);*/
	QObject::connect(mUi->spectrumButton, SIGNAL(clicked()), this, SLOT(showSpectrum()));

	//Correlation
	mUi->matchButton->setIcon(ViThemeManager::image("correlation.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->matchButton->setCheckable(true);
	mCorrelationIndex = addStackWidget(&mCorrelationWidget);
	QObject::connect(mUi->matchButton, SIGNAL(clicked()), this, SLOT(showCorrelation()));
}

ViAnalysisMenu::~ViAnalysisMenu()
{
	delete mUi;
}

void ViAnalysisMenu::showSpectrum()
{
	mUi->matchButton->setChecked(false);
	setCurrentStackIndex(mSpectrumIndex);
}

void ViAnalysisMenu::showCorrelation()
{
	mUi->spectrumButton->setChecked(false);
	setCurrentStackIndex(mCorrelationIndex);
}
