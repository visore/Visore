#include "vimainwidget.h"
#include "ui_vimainwidget.h"

ViMainWidget::ViMainWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainWidget();
	mUi->setupUi(this);

	mUi->originalWaveWidget->setMode(ViAudio::AudioInput);
	mUi->correctedWaveWidget->setMode(ViAudio::AudioOutput);
}

ViMainWidget::~ViMainWidget()
{
	delete mUi;
}
