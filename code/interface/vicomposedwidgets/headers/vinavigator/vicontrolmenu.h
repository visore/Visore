#ifndef VICONTROLMENU_H
#define VICONTROLMENU_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViControlMenu;
}

class ViControlMenu : public ViWidget
{
	Q_OBJECT

	public:
		ViControlMenu(QWidget *parent = 0);
		~ViControlMenu();

	private:
		Ui::ViControlMenu *mUi;
};

#endif
