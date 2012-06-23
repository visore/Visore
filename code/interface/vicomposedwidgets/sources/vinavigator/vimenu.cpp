#include "vimenu.h"
#include "vinavigator.h"

ViMenu::ViMenu(ViNavigator *navigator)
	: ViWidget(navigator)
{
	mNavigator = navigator;
	mCurrentStackIndex = 0;
}

void ViMenu::setCurrentStackIndex(int index)
{
	mCurrentStackIndex = index;
	mNavigator->setStackIndex(index);
}

int ViMenu::currentStackIndex()
{
	return mCurrentStackIndex;
}

int ViMenu::addStackWidget(ViWidget *widget)
{
	return mNavigator->addStackWidget(widget);
}
