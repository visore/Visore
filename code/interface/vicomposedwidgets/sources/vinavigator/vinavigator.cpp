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
}

void ViNavigator::setStackIndex(int index)
{
	mUi->stack->setCurrentIndex(index);
}

int ViNavigator::addStackWidget(ViWidget *widget)
{
	return mUi->stack->addWidget(widget);
}

void ViNavigator::initialize()
{
	mUi->menu->setHeight(120);
	mUi->menu->setRounding(0, 5);
	mUi->menu->setTabOffset(5);

	mInputMenu = new ViInputMenu(this);
	mUi->menu->addTab("Input", mInputMenu);

	mControlMenu = new ViControlMenu(this);
	mUi->menu->addTab("Control", mControlMenu);

	mOutputMenu = new ViOutputMenu(this);
	mUi->menu->addTab("Output", mOutputMenu);

	mAnalysisMenu = new ViAnalysisMenu(this);
	mUi->menu->addTab("Analysis", mAnalysisMenu);
}
