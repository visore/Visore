#include "vianalysismenu.h"
#include "ui_vianalysismenu.h"

ViAnalysisMenu::ViAnalysisMenu(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViAnalysisMenu();
	mUi->setupUi(this);
}

ViAnalysisMenu::~ViAnalysisMenu()
{
	delete mUi;
}
