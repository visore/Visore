#include <viprojectrecordingwidget.h>
#include <ui_viprojectrecordingwidget.h>
#include <viprojectmetadatawidget.h>
#include <vistackedwidget.h>
#include <vimanager.h>
#include <QDate>

ViProjectRecordingWidget::ViProjectRecordingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectRecordingWidget();
	mUi->setupUi(this);

	QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(finishRecording()));

	mProject = NULL;

	mUi->existingBrowser->addFilter(ViManager::projectFilter());
	mUi->existingBrowser->setMode(ViFileBrowser::OpenFile);
	mUi->existingBrowser->setDirectory(ViManager::projectPath());
	mUi->newBrowser->addFilter(ViManager::projectFilter());
	mUi->newBrowser->setMode(ViFileBrowser::SaveFile);

	QString date = QDate::currentDate().toString("dd-MM-yyyy");
	mUi->nameLineEdit->setText("Visore Project (" + date + ")");
	mUi->newBrowser->setFileName(ViManager::projectPath() + "Visore Project (" + date + ")." + ViManager::projectExtension());

	mUi->startButton->setIcon(ViThemeManager::icon("microphone"), 36);
	QObject::connect(mUi->startButton, SIGNAL(clicked()), this, SLOT(load()));

	QObject::connect(mUi->newRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	QObject::connect(mUi->existingRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	changeType();

	setStyleSheet(styleSheet() + "QLabel { width: 150px; min-width: 150px; }");
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

    ViAudio::Type type = ViAudio::Target;
	if(mUi->corruptedRadioButton->isChecked())
	{
		type = ViAudio::Corrupted;
	}

    int sides = mUi->sidesSpinBox->value();
    if(mUi->existingRadioButton->isChecked())
    {
        sides = mProject->sideCount();
    }

	engine()->recordProject(mProject, type, mUi->formatWidget->format(), sides, mUi->detectBox->isChecked());
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

void ViProjectRecordingWidget::finishRecording()
{
	if(mUi->editBox->isChecked())
	{
		ViProjectMetadataWidget *widget = dynamic_cast<ViProjectMetadataWidget*>(ViStackedWidget::widget("ViProjectMetadataWidget"));
		if(widget != NULL)
		{
			widget->setProject(mProject);
			ViStackedWidget::setCurrentWidget(widget);
		}
	}
}
