#include "vioutputmenu.h"
#include "ui_vioutputmenu.h"

ViOutputMenu::ViOutputMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViOutputMenu();
	mUi->setupUi(this);
}

ViOutputMenu::~ViOutputMenu()
{
	delete mUi;
}
