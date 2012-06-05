#include "viinputmenu.h"
#include "ui_viinputmenu.h"

ViInputMenu::ViInputMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViInputMenu();
	mUi->setupUi(this);
}

ViInputMenu::~ViInputMenu()
{
	delete mUi;
}
