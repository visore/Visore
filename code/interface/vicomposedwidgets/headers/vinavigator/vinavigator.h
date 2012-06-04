#ifndef VINAVIGATOR_H
#define VINAVIGATOR_H

#include "vitabwidget.h"
#include "vicontrolmenu.h"
#include "viinputmenu.h"
#include "vioutputmenu.h"
#include "vianalysismenu.h"
#include "vicontrolcontainer.h"
#include "viinputcontainer.h"
#include "vioutputcontainer.h"
#include "vianalysiscontainer.h"

namespace Ui
{
    class ViNavigator;
}

class ViNavigator : public ViWidget
{

	public:

		ViNavigator(QWidget *parent = 0);
		~ViNavigator();

	protected:

		void initialize();

	private:

		Ui::ViNavigator *mUi;

		ViControlMenu *mControlMenu;
		ViInputMenu *mInputMenu;
		ViOutputMenu *mOutputMenu;
		ViAnalysisMenu *mAnalysisMenu;

		ViControlContainer *mControlContainer;
		ViInputContainer *mInputContainer;
		ViOutputContainer *mOutputContainer;
		ViAnalysisContainer *mAnalysisContainer;

};

#endif
