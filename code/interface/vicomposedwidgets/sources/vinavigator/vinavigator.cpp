#include "vinavigator.h"
#include "ui_vinavigator.h"

ViNavigator::ViNavigator(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViNavigator();
	mUi->setupUi(this);
	initialize();
}

ViNavigator::~ViNavigator()
{
	if(mInputMenu != NULL)
	{
		delete mInputMenu;
	}
	if(mOutputMenu != NULL)
	{
		delete mOutputMenu;
	}
	if(mControlMenu != NULL)
	{
		delete mControlMenu;
	}
	if(mAnalysisMenu != NULL)
	{
		delete mAnalysisMenu;
	}
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

void ViNavigator::initialize()
{
	mUi->menu->setHeight(120);
	mUi->menu->setRounding(0, 5);
	mUi->menu->setTabOffset(5);

	mInputMenu = new ViInputMenu(this);
	mUi->menu->addTab("Input", mInputMenu);
	mInputContainer = new ViInputContainer(this);
	mUi->stack->addWidget(mInputContainer);

	mControlMenu = new ViControlMenu(this);
	mUi->menu->addTab("Control", mControlMenu);
	mControlContainer = new ViControlContainer(this);
	mUi->stack->addWidget(mControlContainer);

	mOutputMenu = new ViOutputMenu(this);
	mUi->menu->addTab("Output", mOutputMenu);
	mOutputContainer = new ViOutputContainer(this);
	mUi->stack->addWidget(mOutputContainer);

	mAnalysisMenu = new ViAnalysisMenu(this);
	mUi->menu->addTab("Analysis", mAnalysisMenu);
	mAnalysisContainer = new ViAnalysisContainer(this);
	mUi->stack->addWidget(mAnalysisContainer);
}
