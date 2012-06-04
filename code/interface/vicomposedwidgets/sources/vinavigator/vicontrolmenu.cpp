#include "vicontrolmenu.h"
#include "ui_vicontrolmenu.h"

ViControlMenu::ViControlMenu(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViControlMenu();
	mUi->setupUi(this);
}

ViControlMenu::~ViControlMenu()
{
	delete mUi;
}
