#include "viinputcontainer.h"
#include "ui_viinputcontainer.h"

ViInputContainer::ViInputContainer(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputContainer();
	mUi->setupUi(this);
}

ViInputContainer::~ViInputContainer()
{
	delete mUi;
}

void ViInputContainer::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	mUi->inputWidget->setEngine(mEngine);
}
