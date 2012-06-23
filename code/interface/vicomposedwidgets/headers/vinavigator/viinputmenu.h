#ifndef VIINPUTMENU_H
#define VIINPUTMENU_H

#include "vimenu.h"
#include "virecordingprojectwidget.h"

namespace Ui
{
    class ViInputMenu;
}

class ViInputMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void showRecordingProject();

	public:

		ViInputMenu(ViNavigator *navigator = 0);
		~ViInputMenu();

	private:

		Ui::ViInputMenu *mUi;

		ViRecordingProjectWidget mRecordingProjectWidget;

		qint16 mRecordingProjectIndex;

};

#endif
