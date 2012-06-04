#include "vioutputmenu.h"
#include "ui_vioutputmenu.h"

ViOutputMenu::ViOutputMenu(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViOutputMenu();
	mUi->setupUi(this);
}

ViOutputMenu::~ViOutputMenu()
{
	delete mUi;
}
