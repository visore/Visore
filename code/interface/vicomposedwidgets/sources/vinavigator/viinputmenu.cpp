#include "viinputmenu.h"
#include "ui_viinputmenu.h"

ViInputMenu::ViInputMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViInputMenu();
	mUi->setupUi(this);

	mUi->recordingProjectButton->setIcon(ViThemeManager::image("recordingproject.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->recordingProjectButton->setCheckable(true);
	mRecordingProjectIndex = addStackWidget(&mRecordingProjectWidget);
	QObject::connect(mUi->recordingProjectButton, SIGNAL(clicked()), this, SLOT(showRecordingProject()));
}

ViInputMenu::~ViInputMenu()
{
	delete mUi;
}

void ViInputMenu::showRecordingProject()
{
	setCurrentStackIndex(mRecordingProjectIndex);
}
