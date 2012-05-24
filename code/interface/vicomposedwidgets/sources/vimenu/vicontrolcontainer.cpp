#include "vicontrolcontainer.h"
#include "ui_vicontrolcontainer.h"

ViControlContainer::ViControlContainer(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViControlContainer();
	mUi->setupUi(this);
}

ViControlContainer::~ViControlContainer()
{
	delete mUi;
}
