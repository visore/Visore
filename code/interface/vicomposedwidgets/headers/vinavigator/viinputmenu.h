#ifndef VIINPUTMENU_H
#define VIINPUTMENU_H

#include "vimenu.h"
#include "viprojectwidget.h"

namespace Ui
{
    class ViInputMenu;
}

class ViInputMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void selectProject();
		void selectRecording();
		void selectFile();

	public:

		ViInputMenu(ViNavigator *navigator = 0);
		~ViInputMenu();

	private:

		Ui::ViInputMenu *mUi;

		ViProjectWidget mProjectWidget;

		qint16 mProjectIndex;

};

#endif
