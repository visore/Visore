#ifndef VIINPUTMENU_H
#define VIINPUTMENU_H

#include "vimenu.h"

namespace Ui
{
    class ViInputMenu;
}

class ViInputMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void selectRecording();
		void selectFile();

	public:

		ViInputMenu(ViNavigator *navigator = 0);
		~ViInputMenu();

	private:

		Ui::ViInputMenu *mUi;

};

#endif
