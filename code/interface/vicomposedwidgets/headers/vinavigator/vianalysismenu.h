#ifndef VIANALYSISMENU_H
#define VIANALYSISMENU_H

#include "vithememanager.h"
#include "vispectrumwidget.h"
#include "vimatchwidget.h"

class ViNavigator;

namespace Ui
{
    class ViAnalysisMenu;
}

class ViAnalysisMenu : public ViWidget
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
		ViNavigator *mNavigator;

		ViSpectrumWidget mSpectrumWidget;
		ViMatchWidget mMatchWidget;

		qint16 mSpectrumIndex;
		qint16 mMatchIndex;
};

#endif
