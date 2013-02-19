#ifndef VIMAINMENU_H
#define VIMAINMENU_H

#include <viwidget.h>

namespace Ui
{
    class ViMainMenu;
}

class ViMainMenu : public ViWidget
{

	Q_OBJECT

	public:

		ViMainMenu(QWidget *parent = 0);
		~ViMainMenu();

	private:

		Ui::ViMainMenu *mUi;

};

#endif
