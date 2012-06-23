#include "virecordingprojectwidget.h"
#include "ui_virecordingprojectwidget.h"

ViRecordingProjectWidget::ViRecordingProjectWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViRecordingProjectWidget();
	mUi->setupUi(this);
}

ViRecordingProjectWidget::~ViRecordingProjectWidget()
{
	delete mUi;
}
