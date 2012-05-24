#ifndef VIMENU_H
#define VIMENU_H

#include "viaudioengine.h"
#include "vitabwidget.h"
#include "vicontrolcontainer.h"
#include "viinputcontainer.h"
#include "vioutputcontainer.h"
#include "vianalysiscontainer.h"

class ViMenu : public ViTabWidget
{

	public:

		ViMenu(QWidget *parent = 0);
		~ViMenu();

	protected:

		void initialize();

	private:

		ViControlContainer *mControlContainer;
		ViInputContainer *mInputContainer;
		ViOutputContainer *mOutputContainer;
		ViAnalysisContainer *mAnalysisContainer;

};

#endif
