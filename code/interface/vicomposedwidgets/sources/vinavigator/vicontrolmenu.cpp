#include "vicontrolmenu.h"
#include "ui_vicontrolmenu.h"

ViControlMenu::ViControlMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViControlMenu();
	mUi->setupUi(this);

	//Spectrum analysis widget
	mMainIndex = addStackWidget(&mMainWidget);

	//Set default view
	setCurrentStackIndex(mMainIndex);
}

ViControlMenu::~ViControlMenu()
{
	delete mUi;
}
