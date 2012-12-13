#include "viprojectrecordingwidget.h"
#include "ui_viprojectrecordingwidget.h"
#include "vimanager.h"
#include <QDate>

ViProjectRecordingWidget::ViProjectRecordingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectRecordingWidget();
	mUi->setupUi(this);

	mUi->existingBrowser->addFilter(ViManager::projectFilter());
	mUi->existingBrowser->setMode(ViFileBrowser::OpenFile);
	mUi->existingBrowser->setDirectory(ViManager::projectPath());
	mUi->newBrowser->addFilter(ViManager::projectFilter());
	mUi->newBrowser->setMode(ViFileBrowser::SaveFile);

	QString date = QDate::currentDate().toString("dd-MM-yyyy");
	mUi->nameLineEdit->setText("Visore Project (" + date + ")");
	mUi->newBrowser->setFileName(ViManager::projectPath() + QDir::separator() + "Visore Project (" + date + ")." + ViManager::projectExtension());

	mUi->startButton->setIcon(ViThemeManager::image("start.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	QObject::connect(mUi->startButton, SIGNAL(clicked()), this, SLOT(start()));

	QObject::connect(mUi->newRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	QObject::connect(mUi->existingRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeType()));
	changeType();
}

ViProjectRecordingWidget::~ViProjectRecordingWidget()
{
	delete mUi;
}

void ViProjectRecordingWidget::start()
{
	ViAudio::Type type = ViAudio::TargetType;
	if(mUi->corruptedRadioButton->isChecked())
	{
		type = ViAudio::CorruptedType;
	}
	engine()->startProject(mUi->nameLineEdit->text(), mUi->newBrowser->fileName(), mUi->formatWidget->format(), mUi->sidesSpinBox->value(), type, mUi->existingRadioButton->isChecked());
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
