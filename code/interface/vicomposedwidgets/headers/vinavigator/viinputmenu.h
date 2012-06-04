#ifndef VIINPUTMENU_H
#define VIINPUTMENU_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViInputMenu;
}

class ViInputMenu : public ViWidget
{
	Q_OBJECT

	public:
		ViInputMenu(QWidget *parent = 0);
		~ViInputMenu();

	private:
		Ui::ViInputMenu *mUi;
};

#endif
