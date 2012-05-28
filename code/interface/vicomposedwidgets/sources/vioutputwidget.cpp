#include "vioutputwidget.h"
#include "ui_vioutputwidget.h"

ViOutputWidget::ViOutputWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViOutputWidget();
	mUi->setupUi(this);
	mUi->fileOutputButton->setIcon(ViThemeManager::image("fileinput.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->fileOutputButton->setGlow(ViThemeManager::color(14));
	ViObject::connect(mUi->fileOutputButton, SIGNAL(clicked()), this, SLOT(selectFileOutput()));

	mDialog = new QDialog(mParent);
	mLayout = new QGridLayout(mDialog);
	mLayout->addWidget(new ViOutputControlWidget);
}

ViOutputWidget::~ViOutputWidget()
{
	delete mUi;
	delete mLayout;
	delete mDialog;
}

void ViOutputWidget::selectFileOutput()
{
	mDialog->show();
}
