#include "vivolumebarwidget.h"
#include "ui_vivolumebarwidget.h"

ViVolumeBarWidget::ViVolumeBarWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mUi = new Ui::ViVolumeBarWidget();
	mUi->setupUi(this);
	mUi->volumeBar->setValue(mEngine->volume());
	volumeChanged();

	mUi->muteButton->setCheckable();
	mUi->muteButton->setNormalIcon(ViThemeManager::image("mute.png", ViThemeManager::Normal));
	mUi->muteButton->setHoverIcon(ViThemeManager::image("mute.png", ViThemeManager::Hover));
	mUi->muteButton->setSelectedIcon(ViThemeManager::image("unmute.png", ViThemeManager::Selected));

	setMinimumSize(100, 30);
	setMaximumSize(100, 30);
	mUi->volumeBar->setSize(70, 30);

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
		mUi->muteButton->setHoverIcon(ViThemeManager::image("mute.png", ViThemeManager::Hover));
	}
	else
	{
		mUi->muteButton->setHoverIcon(ViThemeManager::image("unmute.png", ViThemeManager::Hover));
	}
}
