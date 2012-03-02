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
	mUi->muteButton->setNormalIcon(ViThemeManager::image("unmute.png"));
	mUi->muteButton->setCheckedIcon(ViThemeManager::image("mute.png"));
	setMinimumSize(100, 30);
	setMaximumSize(100, 30);
	mUi->volumeBar->setSize(70, 30);

	ViObject::connect(mUi->volumeBar, SIGNAL(valueChanged(int)), mEngine, SLOT(setVolume(int)));
	ViObject::connect(mUi->muteButton, SIGNAL(clicked(bool)), mEngine, SLOT(mute(bool)));
}

ViVolumeBarWidget::~ViVolumeBarWidget()
{
	delete mUi;
}

void ViVolumeBarWidget::volumeChanged(int volume)
{
	volume = mEngine->volume();
}
