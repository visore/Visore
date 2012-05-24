#include "viinputwidget.h"
#include "ui_viinputwidget.h"

ViInputWidget::ViInputWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViInputWidget();
	mUi->setupUi(this);
	mUi->vinylInputButton->setIcon(ViThemeManager::image("vinylinput.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->fileInputButton->setIcon(ViThemeManager::image("fileinput.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->vinylInputButton->setIcon(ViThemeManager::image("vinylinput.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->fileInputButton->setIcon(ViThemeManager::image("fileinput.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->vinylInputButton->setCheckable(true);
	mUi->fileInputButton->setCheckable(true);
	mUi->vinylInputButton->setGlow(ViThemeManager::color(14));
	mUi->fileInputButton->setGlow(ViThemeManager::color(14));

	ViObject::connect(mUi->vinylInputButton, SIGNAL(clicked()), this, SLOT(selectLineInput()));
	ViObject::connect(mUi->fileInputButton, SIGNAL(clicked()), this, SLOT(selectFileInput()));
}

ViInputWidget::~ViInputWidget()
{
	delete mUi;
}

void ViInputWidget::selectLineInput()
{
	mUi->fileInputButton->setChecked(false);
	mEngine->reset();
	mEngine->setInput(ViAudioEngine::Stream);
}

void ViInputWidget::selectFileInput()
{
	//QString file = QFileDialog::getOpenFileName(this, "Open Audio File", QDir::homePath());
	QString file = "/home/visore/a.wav";
	//QString file = "/home/visore/a.flac";
	if(file != "")
	{
		mUi->vinylInputButton->setChecked(false);
		mEngine->reset();
		mEngine->setInputFilePath(file);
		mEngine->setInput(ViAudioEngine::File);
	}
	else
	{
		mUi->fileInputButton->setChecked(false);
	}
}
