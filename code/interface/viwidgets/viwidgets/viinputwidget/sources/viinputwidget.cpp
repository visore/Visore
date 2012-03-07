#include "viinputwidget.h"
#include "ui_viinputwidget.h"

ViInputWidget::ViInputWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputWidget();
	mUi->setupUi(this);
}

ViInputWidget::~ViInputWidget()
{
	delete mUi;
}

void ViInputWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);

	ViObject::connect(mUi->lineInput, SIGNAL(clicked()), this, SLOT(selectLineInput()));
	ViObject::connect(mUi->fileInput, SIGNAL(clicked()), this, SLOT(selectFileInput()));
}

void ViInputWidget::selectLineInput()
{
}

void ViInputWidget::selectFileInput()
{
	mEngine->setInputFilePath("/home/visore/Desktop/a.wav");
}
