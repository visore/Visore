#include "vimenu.h"

ViMenu::ViMenu(QWidget *parent)
	: ViTabWidget(parent)
{
	initialize();
}

ViMenu::~ViMenu()
{
	if(mInputContainer != NULL)
	{
		delete mInputContainer;
	}
	if(mOutputContainer != NULL)
	{
		delete mOutputContainer;
	}
	if(mControlContainer != NULL)
	{
		delete mControlContainer;
	}
	if(mAnalysisContainer != NULL)
	{
		delete mAnalysisContainer;
	}
}

void ViMenu::initialize()
{
	setHeight(120);
	setRounding(0, 5);
	setTabOffset(5);

	mInputContainer = new ViInputContainer(this);
	addTab("Input", mInputContainer);

	mControlContainer = new ViControlContainer(this);
	addTab("Control", mControlContainer);

	mOutputContainer = new ViOutputContainer(this);
	addTab("Output", mOutputContainer);

	mAnalysisContainer = new ViAnalysisContainer(this);
	addTab("Analysis", mAnalysisContainer);
}
