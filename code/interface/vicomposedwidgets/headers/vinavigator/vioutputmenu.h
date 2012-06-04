#ifndef VIOUTPUTMENU_H
#define VIOUTPUTMENU_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViOutputMenu;
}

class ViOutputMenu : public ViWidget
{
	Q_OBJECT

	public:
		ViOutputMenu(QWidget *parent = 0);
		~ViOutputMenu();

	private:
		Ui::ViOutputMenu *mUi;
};

#endif
