#include "vicontrolmenu.h"
#include "ui_vicontrolmenu.h"

ViControlMenu::ViControlMenu(ViNavigator *navigator)
	: ViMenu(navigator)
{
	mUi = new Ui::ViControlMenu();
	mUi->setupUi(this);

	//Spectrum analysis widget
	mMainIndex = addStackWidget(&mMainWidget);

	QObject::connect(mUi->playbackWidget, SIGNAL(clicked()), this, SLOT(select()));
}

ViControlMenu::~ViControlMenu()
{
	delete mUi;
}

void ViControlMenu::select()
{
	setCurrentStackIndex(mMainIndex);
}
