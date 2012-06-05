#ifndef VIMENU_H
#define VIMENU_H

#include "viwidget.h"

class ViNavigator;

class ViMenu : public ViWidget
{
	
	public:

		ViMenu(ViNavigator *navigator = 0);
		void setCurrentStackIndex(int index);
		int currentStackIndex();

	protected:

		int addStackWidget(ViWidget *widget);
		void setStackIndex(int index);

	protected:

		ViNavigator *mNavigator;
		int mCurrentStackIndex;

};

#endif
