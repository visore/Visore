#ifndef VIOUTPUTMENU_H
#define VIOUTPUTMENU_H

#include "vimenu.h"

namespace Ui
{
    class ViOutputMenu;
}

class ViOutputMenu : public ViMenu
{
	Q_OBJECT

	public:
		ViOutputMenu(ViNavigator *navigator = 0);
		~ViOutputMenu();

	private:
		Ui::ViOutputMenu *mUi;
};

#endif
