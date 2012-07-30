#ifndef VIANALYSISMENU_H
#define VIANALYSISMENU_H

#include "vimenu.h"
#include "vithememanager.h"
#include "vispectrumanalysiswidget.h"
#include "vicorrelationwidget.h"

namespace Ui
{
    class ViAnalysisMenu;
}

class ViAnalysisMenu : public ViMenu
{
	Q_OBJECT

	private slots:

		void showSpectrum();
		void showCorrelation();

	public:
		ViAnalysisMenu(ViNavigator *navigator = 0);
		~ViAnalysisMenu();

	private:
		Ui::ViAnalysisMenu *mUi;

		//ViSpectrumWidget mSpectrumWidget;
		ViCorrelationWidget mCorrelationWidget;

		qint16 mSpectrumIndex;
		qint16 mCorrelationIndex;
};

#endif
