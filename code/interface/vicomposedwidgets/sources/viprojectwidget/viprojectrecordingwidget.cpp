#include "viprojectrecordingwidget.h"
#include "ui_viprojectrecordingwidget.h"
#include <QDate>

ViProjectRecordingWidget::ViProjectRecordingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectRecordingWidget();
	mUi->setupUi(this);

	QString date = QDate::currentDate().toString("dd-MM-yyyy");
	mUi->nameLineEdit->setText("Visore Project (" + date + ")");
	mUi->fileBrowser->setFileName(ViManager::projectPath() + QDir::separator() + "Visore Project (" + date + ")." + ViManager::projectExtension());

	mUi->startButton->setIcon(ViThemeManager::image("start.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	QObject::connect(mUi->startButton, SIGNAL(clicked()), this, SLOT(start()));
}

ViProjectRecordingWidget::~ViProjectRecordingWidget()
{
	delete mUi;
}

void ViProjectRecordingWidget::start()
{
	engine()->startProject(mUi->nameLineEdit->text(), mUi->fileBrowser->fileName(), mUi->formatWidget->format(), mUi->sidesSpinBox->value(), mUi->playCheckBox->isChecked());
}
