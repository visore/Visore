#include "vivolumedialwidget.h"
#include "ui_vivolumedialwidget.h"

ViVolumeDialWidget::ViVolumeDialWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mUi = new Ui::ViVolumeDialWidget();
	mUi->setupUi(this);
	ViObject::connect(mUi->volumeDial, SIGNAL(valueChanged(int)), mEngine, SLOT(setVolume(int)));
	ViObject::connect(mUi->volumeDial, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
	volumeChanged();
	mUi->volumeDial->setSize(80, 80);
	mUi->muteButton->setCheckable();
	mUi->muteButton->setNormalIcon(ViThemeManager::image("unmute.png"));
	mUi->muteButton->setCheckedIcon(ViThemeManager::image("mute.png"));
	setMinimumSize(100, 120);
	setMaximumSize(100, 120);
}

ViVolumeDialWidget::~ViVolumeDialWidget()
{
	delete mUi;
}

void ViVolumeDialWidget::volumeChanged(int volume)
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
