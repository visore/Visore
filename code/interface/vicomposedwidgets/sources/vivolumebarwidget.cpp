#include "vivolumebarwidget.h"
#include "ui_vivolumebarwidget.h"

ViVolumeBarWidget::ViVolumeBarWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViVolumeBarWidget();
	mUi->setupUi(this);

	mUi->muteButton->setCheckable(true);
	mUi->muteButton->setIcon(ViThemeManager::image("mute.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->muteButton->setIcon(ViThemeManager::image("mute.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Hovered);
	mUi->muteButton->setIcon(ViThemeManager::image("unmute.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Selected);

	setMinimumSize(100, 30);
	setMaximumSize(100, 30);
	mUi->volumeBar->setSize(70, 30);

	mUi->volumeBar->setValue(mEngine->volume());
	volumeChanged();

	ViObject::connect(mUi->volumeBar, SIGNAL(valueChanged(int)), mEngine, SLOT(setVolume(int)));
	ViObject::connect(mUi->muteButton, SIGNAL(clicked(bool)), this, SLOT(mute(bool)));
}

ViVolumeBarWidget::~ViVolumeBarWidget()
{
	delete mUi;
}

void ViVolumeBarWidget::volumeChanged(int volume)
{
	volume = mEngine->volume();
}

void ViVolumeBarWidget::mute(bool value)
{
	mEngine->mute(value);
	if(value)
	{
		mUi->muteButton->setIcon(ViThemeManager::image("unmute.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Hovered);
	}
	else
	{
		mUi->muteButton->setIcon(ViThemeManager::image("mute.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Hovered);
	}
}
