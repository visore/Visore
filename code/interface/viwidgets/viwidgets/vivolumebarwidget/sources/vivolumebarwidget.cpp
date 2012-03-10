#include "vivolumebarwidget.h"
#include "ui_vivolumebarwidget.h"

ViVolumeBarWidget::ViVolumeBarWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViVolumeBarWidget();
	mUi->setupUi(this);

	mUi->muteButton->setCheckable();
	mUi->muteButton->setNormalIcon(ViThemeManager::image("mute.png", ViThemeManager::Normal));
	mUi->muteButton->setHoverIcon(ViThemeManager::image("mute.png", ViThemeManager::Colored));
	mUi->muteButton->setSelectedIcon(ViThemeManager::image("unmute.png", ViThemeManager::Normal));

	setMinimumSize(100, 30);
	setMaximumSize(100, 30);
	mUi->volumeBar->setSize(70, 30);
}

ViVolumeBarWidget::~ViVolumeBarWidget()
{
	delete mUi;
}

void ViVolumeBarWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);

	mUi->volumeBar->setValue(mEngine->volume());
	volumeChanged();

	ViObject::connect(mUi->volumeBar, SIGNAL(valueChanged(int)), mEngine, SLOT(setVolume(int)));
	ViObject::connect(mUi->muteButton, SIGNAL(clicked(bool)), this, SLOT(mute(bool)));
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
		mUi->muteButton->setHoverIcon(ViThemeManager::image("unmute.png", ViThemeManager::Colored));
	}
	else
	{
		mUi->muteButton->setHoverIcon(ViThemeManager::image("mute.png", ViThemeManager::Colored));
	}
}
