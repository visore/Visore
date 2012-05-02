#include "vioutputcontainer.h"
#include "ui_vioutputcontainer.h"

ViOutputContainer::ViOutputContainer(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViOutputContainer();
	mUi->setupUi(this);
}

ViOutputContainer::~ViOutputContainer()
{
	delete mUi;
}

void ViOutputContainer::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	mUi->outputWidget->setEngine(mEngine);
}
