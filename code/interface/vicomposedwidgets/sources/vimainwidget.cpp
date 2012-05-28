#include "vimainwidget.h"
#include "ui_vimainwidget.h"

ViMainWidget::ViMainWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainWidget();
	mUi->setupUi(this);

	mUi->originalWaveWidget->setDirection(ViAudioConnection::Input);
	mUi->correctedWaveWidget->setDirection(ViAudioConnection::Output);
}

ViMainWidget::~ViMainWidget()
{
	delete mUi;
}
