#ifndef VICONTROLMENU_H
#define VICONTROLMENU_H

#include "vimenu.h"
#include "vimainwidget.h"

namespace Ui
{
    class ViControlMenu;
}

class ViControlMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void select();

	public:

		ViControlMenu(ViNavigator *navigator = 0);
		~ViControlMenu();

	private:

		Ui::ViControlMenu *mUi;

		ViMainWidget mMainWidget;

		qint16 mMainIndex;

};

#endif
