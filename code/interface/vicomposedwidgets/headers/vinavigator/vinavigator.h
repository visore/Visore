#ifndef VINAVIGATOR_H
#define VINAVIGATOR_H

#include "vitabwidget.h"
#include "vicontrolmenu.h"
#include "viinputmenu.h"
#include "vioutputmenu.h"
#include "vianalysismenu.h"

namespace Ui
{
    class ViNavigator;
}

class ViNavigator : public ViWidget
{

	public:

		ViNavigator(QWidget *parent = 0);
		~ViNavigator();
		void setStackIndex(int index);
		int addStackWidget(ViWidget *widget);

	protected:

		void initialize();

	private:

		Ui::ViNavigator *mUi;

		ViControlMenu *mControlMenu;
		ViInputMenu *mInputMenu;
		ViOutputMenu *mOutputMenu;
		ViAnalysisMenu *mAnalysisMenu;

};

#endif
