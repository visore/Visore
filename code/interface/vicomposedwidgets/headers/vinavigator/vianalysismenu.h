#ifndef VIANALYSISMENU_H
#define VIANALYSISMENU_H

#include "vimenu.h"
#include "vithememanager.h"
#include "vispectrumwidget.h"
#include "vimatchwidget.h"

namespace Ui
{
    class ViAnalysisMenu;
}

class ViAnalysisMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void showSpectrum();
		void showMatch();

	public:
		ViAnalysisMenu(ViNavigator *navigator = 0);
		~ViAnalysisMenu();

	private:
		Ui::ViAnalysisMenu *mUi;

		ViSpectrumWidget mSpectrumWidget;
		ViMatchWidget mMatchWidget;

		qint16 mSpectrumIndex;
		qint16 mMatchIndex;
};

#endif
