#include "vinavigator.h"
#include "ui_vinavigator.h"
#include "vicontrolmenu.h"
#include "viinputmenu.h"
#include "vioutputmenu.h"
#include "vianalysismenu.h"

ViNavigator::ViNavigator(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViNavigator();
	mUi->setupUi(this);
	initialize();
}

ViNavigator::~ViNavigator()
{
	qDeleteAll(mMenus);
	mMenus.clear();
}

void ViNavigator::changeStackIndex(int tabIndex)
{
	setStackIndex(mMenus[tabIndex]->currentStackIndex());
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
	ViNavigator::connect(mUi->menu, SIGNAL(tabChanged(int)), this, SLOT(changeStackIndex(int)));

	mUi->menu->setHeight(120);
	mUi->menu->setRounding(0, 5);
	mUi->menu->setTabOffset(5);

	addMenu("Input", new ViInputMenu(this));
	addMenu("Control", new ViControlMenu(this));
	addMenu("Output", new ViOutputMenu(this));
	addMenu("Analysis", new ViAnalysisMenu(this));

	mMenus[0]->setCurrentStackIndex(mMenus[1]->currentStackIndex());
	mMenus[2]->setCurrentStackIndex(mMenus[1]->currentStackIndex());
}

void ViNavigator::addMenu(QString name, ViMenu *menu)
{
	mUi->menu->addTab(name, menu);
	mMenus.append(menu);
}
