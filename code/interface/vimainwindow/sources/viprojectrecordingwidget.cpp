#include "viprojectrecordingwidget.h"
#include "ui_viprojectrecordingwidget.h"
#include "vimanager.h"
#include <QDate>

ViProjectRecordingWidget::ViProjectRecordingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectRecordingWidget();
	mUi->setupUi(this);

	mProject = NULL;

	mUi->existingBrowser->addFilter(ViManager::projectFilter());
	mUi->existingBrowser->setMode(ViFileBrowser::OpenFile);
	mUi->existingBrowser->setDirectory(ViManager::projectPath());
	mUi->newBrowser->addFilter(ViManager::projectFilter());
	mUi->newBrowser->setMode(ViFileBrowser::SaveFile);

	QString date = QDate::currentDate().toString("dd-MM-yyyy");
	mUi->nameLineEdit->setText("Visore Project (" + date + ")");
	mUi->newBrowser->setFileName(ViManager::projectPath() + "Visore Project (" + date + ")." + ViManager::projectExtension());

	mUi->startButton->setSize(64, 64);
	mUi->startButton->setIcon(ViThemeManager::icon("startprocess"), 36);
	QObject::connect(mUi->startButton, SIGNAL(clicked()), this, SLOT(load()));

	QObject::connect(mUi->newRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	QObject::connect(mUi->existingRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	changeType();
}

ViProjectRecordingWidget::~ViProjectRecordingWidget()
{
	delete mUi;
	clear();
}

void ViProjectRecordingWidget::load()
{
	clear();
	mProject = new ViProject();
    QObject::connect(mProject, SIGNAL(cleared()), this, SLOT(clear()));
	if(mUi->existingRadioButton->isChecked())
	{
        mProject->setFilePath(mUi->existingBrowser->fileName());
		QObject::connect(mProject, SIGNAL(loaded()), this, SLOT(start()));
		mProject->load();
	}
	else
    {
        mProject->setProjectName(mUi->nameLineEdit->text());
        mProject->setFilePath(mUi->newBrowser->fileName());
		start();
	}
}

void ViProjectRecordingWidget::start()
{
	QObject::disconnect(mProject, SIGNAL(loaded()), this, SLOT(start()));

    ViAudioObject::Type type = ViAudioObject::Target;
	if(mUi->corruptedRadioButton->isChecked())
	{
		type = ViAudioObject::Corrupted;
	}

    int sides = mUi->sidesSpinBox->value();
    if(mUi->existingRadioButton->isChecked())
    {
        sides = mProject->sideCount();
    }

    engine()->recordProject(mProject, type, mUi->formatWidget->format(), sides, mUi->songInfoBox->isChecked());
}

void ViProjectRecordingWidget::changeType()
{
	if(mUi->newRadioButton->isChecked())
	{
		mUi->existingGroup->hide();
		mUi->newGroup->show();
	}
	else
	{
		mUi->existingGroup->show();
		mUi->newGroup->hide();
	}
}

void ViProjectRecordingWidget::clear()
{
	if(mProject != NULL)
	{
        QObject::disconnect(mProject, SIGNAL(cleared()), this, SLOT(clear()));
		delete mProject;
		mProject = NULL;
	}
}
