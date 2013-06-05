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

	QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(clear()));
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
	mProject->setProjectName(mUi->nameLineEdit->text());
	mProject->setFilePath(mUi->newBrowser->fileName());

	if(mUi->existingRadioButton->isChecked())
	{
		QObject::connect(mProject, SIGNAL(loaded()), this, SLOT(start()));
		mProject->load();
	}
	else
	{
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
	engine()->recordProject(mProject, type, mUi->formatWidget->format(), mUi->sidesSpinBox->value(), mUi->songInfoBox->isChecked());
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
		delete mProject;
		mProject = NULL;
	}
}
