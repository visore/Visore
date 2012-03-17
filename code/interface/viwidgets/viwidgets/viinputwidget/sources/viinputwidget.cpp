#include "viinputwidget.h"
#include "ui_viinputwidget.h"

ViInputWidget::ViInputWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputWidget();
	mUi->setupUi(this);
	mUi->vinylInputButton->setNormalIcon(ViThemeManager::image("vinylinput.png", ViThemeManager::Normal));
	mUi->fileInputButton->setSelectedIcon(ViThemeManager::image("fileinput.png", ViThemeManager::Normal));
	mUi->vinylInputButton->setSelectedIcon(ViThemeManager::image("vinylinput.png", ViThemeManager::Colored));
	mUi->fileInputButton->setNormalIcon(ViThemeManager::image("fileinput.png", ViThemeManager::Colored));
	mUi->vinylInputButton->setCheckable();
	mUi->fileInputButton->setCheckable();
	mUi->vinylInputButton->setGlow(ViThemeManager::color(14));
	mUi->fileInputButton->setGlow(ViThemeManager::color(14));
}

ViInputWidget::~ViInputWidget()
{
	delete mUi;
}

void ViInputWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	ViObject::connect(mUi->vinylInputButton, SIGNAL(clicked()), this, SLOT(selectLineInput()));
	ViObject::connect(mUi->fileInputButton, SIGNAL(clicked()), this, SLOT(selectFileInput()));
}

void ViInputWidget::selectLineInput()
{
}

void ViInputWidget::selectFileInput()
{
	QString file = QFileDialog::getOpenFileName(this, "Open Audio File", QDir::homePath());
	if(file != "")
	{
		mEngine->setInputFilePath(file);
	}
}
