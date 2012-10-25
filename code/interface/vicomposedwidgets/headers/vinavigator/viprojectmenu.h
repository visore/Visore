#ifndef VIPROJECTMENU_H
#define VIPROJECTMENU_H

#include "vimenu.h"
#include "viprojectrecordingwidget.h"
#include "viprojectcorrelationwidget.h"

namespace Ui
{
    class ViProjectMenu;
}

class ViProjectMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void selectRecording();
		void selectCorrelation();

	public:

		ViProjectMenu(ViNavigator *navigator = 0);
		~ViProjectMenu();

	private:

		Ui::ViProjectMenu *mUi;

		ViProjectRecordingWidget mRecordingWidget;
		ViProjectCorrelationWidget mCorrelationWidget;

		qint16 mRecordingIndex;
		qint16 mCorrelationIndex;

};

#endif
