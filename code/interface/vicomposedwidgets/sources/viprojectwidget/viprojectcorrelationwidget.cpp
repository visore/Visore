#include "viprojectcorrelationwidget.h"
#include "ui_viprojectcorrelationwidget.h"
#include "viproject.h"
#include <QEventLoop>

#include "vilogger.h"

ViRemoveWidget::ViRemoveWidget(int row)
{
	mRow = row;
	mButton = new ViPushButton();
	mButton->setIcon(ViThemeManager::image("remove.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal, 20, 20);
	mButton->setSize(30, 30);
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(mButton);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	QObject::connect(mButton, SIGNAL(clicked()), this, SLOT(click()));
}

ViRemoveWidget::~ViRemoveWidget()
{
	delete mButton;
}

int ViRemoveWidget::row()
{
	return mRow;
}

void ViRemoveWidget::setRow(int row)
{
	mRow = row;
}

void ViRemoveWidget::click()
{
	emit clicked(mRow);
}

ViProjectCorrelationWidget::ViProjectCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectCorrelationWidget();
	mUi->setupUi(this);

	mUi->fileBrowser->setMode(ViFileBrowser::OpenFiles);

	mUi->addButton->setIcon(ViThemeManager::image("add.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal, 22, 22);
	mUi->addButton->setSize(34, 34);
	QObject::connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addProjects()));

	mUi->tableWidget->setColumnCount(4);
	mUi->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("File Name"));
	mUi->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Project Name"));
	mUi->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Project Size"));
	mUi->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Remove"));
}

ViProjectCorrelationWidget::~ViProjectCorrelationWidget()
{
	delete mUi;
}

void ViProjectCorrelationWidget::addProjects()
{
	QStringList projects = mUi->fileBrowser->fileNames();
	int row = mUi->tableWidget->rowCount();
	mUi->tableWidget->setRowCount(mUi->tableWidget->rowCount() + projects.size());
	int invalidProjects = 0;
	for(int i = 0; i < projects.size(); ++i)
	{
		QEventLoop loop;
		ViProject project(projects[i]);
		QObject::connect(&project, SIGNAL(finished()), &loop, SLOT(quit()));
		if(project.load(true))
		{
			loop.exec(); // Wait until the archive in the project was extracted
			mUi->tableWidget->setItem(row + i, 0, new QTableWidgetItem(project.fileName()));
			mUi->tableWidget->setItem(row + i, 1, new QTableWidgetItem(project.projectName()));
			mUi->tableWidget->setItem(row + i, 2, new QTableWidgetItem(QString::number(project.size())));
			ViRemoveWidget *remove = new ViRemoveWidget(row + i);
			mButtons.append(remove);
			QObject::connect(remove, SIGNAL(clicked(int)), this, SLOT(remove(int)));
			mUi->tableWidget->setCellWidget(row + i, 3, remove);
		}
		else
		{
			++invalidProjects;
		}
	}
	mUi->tableWidget->setRowCount(mUi->tableWidget->rowCount() - invalidProjects);
}

void ViProjectCorrelationWidget::remove(int row)
{
	for(int i = row + 1; i < mButtons.size(); ++i)
	{
		mButtons[i]->setRow(i - 1);
	}
	mButtons.removeAt(row);
	mUi->tableWidget->removeRow(row);
}

void ViProjectCorrelationWidget::start()
{
	LOG("rrrrrr");
}
