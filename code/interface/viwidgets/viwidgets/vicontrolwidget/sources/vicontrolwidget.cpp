#include "vicontrolwidget.h"
#include "ui_vicontrolwidget.h"

ViControlWidget::ViControlWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViControlWidget();
	mUi->setupUi(this);
}

ViControlWidget::~ViControlWidget()
{
	delete mUi;
}

void ViControlWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	mUi->playbackWidget->setEngine(mEngine);
	mUi->volumeWidget->setEngine(mEngine);
}
