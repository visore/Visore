#ifndef VIANALYSISMENU_H
#define VIANALYSISMENU_H

#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViAnalysisMenu;
}

class ViAnalysisMenu : public ViWidget
{
	Q_OBJECT

	public:
		ViAnalysisMenu(QWidget *parent = 0);
		~ViAnalysisMenu();

	private:
		Ui::ViAnalysisMenu *mUi;
};

#endif
