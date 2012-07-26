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
	QObject::connect(mUi->recordingProjectButton, SIGNAL(clicked()), this, SLOT(selectRecordingProject()));

	mUi->recordingButton->setIcon(ViThemeManager::image("vinylinput.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->recordingButton->setCheckable(true);
	QObject::connect(mUi->recordingButton, SIGNAL(clicked()), this, SLOT(selectRecording()));

	mUi->fileButton->setIcon(ViThemeManager::image("fileinput.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->fileButton->setCheckable(true);
	QObject::connect(mUi->fileButton, SIGNAL(clicked()), this, SLOT(selectFile()));
}

ViInputMenu::~ViInputMenu()
{
	delete mUi;
}

void ViInputMenu::selectRecordingProject()
{
	setCurrentStackIndex(mRecordingProjectIndex);
	mUi->recordingButton->setChecked(false);
	mUi->fileButton->setChecked(false);
}

void ViInputMenu::selectRecording()
{
	mEngine->changeInput(ViAudio::Line);
	mUi->recordingProjectButton->setChecked(false);
	mUi->fileButton->setChecked(false);
}

void ViInputMenu::selectFile()
{
	mEngine->changeInput(ViAudio::File);
	//QString file = QFileDialog::getOpenFileName(this, "Open Audio File", QDir::homePath());
	QString file = "/home/visore/Project/Data/a.wav";
	if(file != "")
	{
		mUi->recordingProjectButton->setChecked(false);
		mUi->recordingButton->setChecked(false);
		mEngine->openFile(file);
	}
	else
	{
		mUi->fileButton->setChecked(false);
	}
}
