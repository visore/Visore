#include "viprojectmenu.h"
#include "ui_viprojectmenu.h"

ViProjectMenu::ViProjectMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViProjectMenu();
	mUi->setupUi(this);

	mUi->recordingButton->setIcon(ViThemeManager::image("recordingproject.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->recordingButton->setCheckable(true);
	mRecordingIndex = addStackWidget(&mRecordingWidget);
	QObject::connect(mUi->recordingButton, SIGNAL(clicked()), this, SLOT(selectRecording()));

	mUi->correlationButton->setIcon(ViThemeManager::image("correlationproject.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->correlationButton->setCheckable(true);
	mCorrelationIndex = addStackWidget(&mCorrelationWidget);
	QObject::connect(mUi->correlationButton, SIGNAL(clicked()), this, SLOT(selectCorrelation()));
}

ViProjectMenu::~ViProjectMenu()
{
	delete mUi;
}

void ViProjectMenu::selectRecording()
{
	setCurrentStackIndex(mRecordingIndex);
	mUi->correlationButton->setChecked(false);
}

void ViProjectMenu::selectCorrelation()
{
	setCurrentStackIndex(mCorrelationIndex);
	mUi->recordingButton->setChecked(false);
}
