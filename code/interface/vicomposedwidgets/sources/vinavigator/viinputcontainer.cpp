#include "viinputcontainer.h"
#include "ui_viinputcontainer.h"

ViInputContainer::ViInputContainer(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputContainer();
	mUi->setupUi(this);

	mUi->originalWaveWidget->setBufferType(ViAudioBuffer::Original);
	mUi->correctedWaveWidget->setBufferType(ViAudioBuffer::Corrected);
}

ViInputContainer::~ViInputContainer()
{
	delete mUi;
}
