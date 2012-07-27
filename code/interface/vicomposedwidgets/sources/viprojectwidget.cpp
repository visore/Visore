#include "viprojectwidget.h"
#include "ui_viprojectwidget.h"
#include <QDate>

ViProjectWidget::ViProjectWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectWidget();
	mUi->setupUi(this);

	QString date = QDate::currentDate().toString("dd-MM-yyyy");
	mUi->nameLineEdit->setText("Visore Project (" + date + ")");
	mUi->fileBrowser->setFileName(ViManager::projectPath() + QDir::separator() + "Visore Project (" + date + ")." + ViManager::projectExtension());

	QObject::connect(mUi->startButton, SIGNAL(clicked()), this, SLOT(start()));
}

ViProjectWidget::~ViProjectWidget()
{
	delete mUi;
}

void ViProjectWidget::start()
{
	engine()->startProject(mUi->nameLineEdit->text(), mUi->fileBrowser->fileName(), mUi->formatWidget->format());
}
