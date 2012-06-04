#include "viinputmenu.h"
#include "ui_viinputmenu.h"

ViInputMenu::ViInputMenu(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputMenu();
	mUi->setupUi(this);
}

ViInputMenu::~ViInputMenu()
{
	delete mUi;
}
