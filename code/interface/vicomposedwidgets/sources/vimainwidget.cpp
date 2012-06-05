#include "vimainwidget.h"
#include "ui_vimainwidget.h"

ViMainWidget::ViMainWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainWidget();
	mUi->setupUi(this);

	mUi->originalWaveWidget->setBufferType(ViAudioBuffer::Original);
	mUi->correctedWaveWidget->setBufferType(ViAudioBuffer::Corrected);
}

ViMainWidget::~ViMainWidget()
{
	delete mUi;
}
