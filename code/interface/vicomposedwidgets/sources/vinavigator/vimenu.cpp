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
}

int ViMenu::currentStackIndex()
{
	return mCurrentStackIndex;
}

int ViMenu::addStackWidget(ViWidget *widget)
{
	return mNavigator->addStackWidget(widget);
}

void ViMenu::setStackIndex(int index)
{
	mNavigator->setStackIndex(index);
}
