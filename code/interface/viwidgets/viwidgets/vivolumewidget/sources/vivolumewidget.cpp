#include "vivolumewidget.h"
#include "ui_vivolumewidget.h"

ViVolumeWidget::ViVolumeWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mUi = new Ui::ViVolumeWidget();
	mUi->setupUi(this);
	ViObject::connect(mUi->volumeDial, SIGNAL(valueChanged(int)), mEngine, SLOT(setVolume(int)));
	ViObject::connect(mUi->volumeDial, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
	volumeChanged();
	mUi->muteButton->setNormalIcon(ViThemeManager::image("unmute.png"));
	mUi->muteButton->setCheckedIcon(ViThemeManager::image("mute.png"));
}

ViVolumeWidget::~ViVolumeWidget()
{
	delete mUi;
}

void ViVolumeWidget::volumeChanged(int volume)
{
	volume = mEngine->volume();
	mUi->volumeDial->setValue(volume);
	QString text = QString::number(volume)+"%";
	if(volume < 100)
	{
		text = " " + text;
	}
	if(volume < 10)
	{
		text = " " + text;
	}
	mUi->volumeLabel->setText(text);
}
