#include <vivolumewidget.h>
#include <ui_vivolumewidget.h>

ViVolumeWidget::ViVolumeWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViVolumeWidget();
	mUi->setupUi(this);

	setSize(130, 40);
	mUi->volumeBar->setSize(100, 40);

	mUi->muteButton->setCheckable(true);
	mUi->muteButton->disableBackground();
	mUi->muteButton->disableBorder();
	mUi->muteButton->setIcon(ViThemeManager::icon("mute"), 24);

	QObject::connect(mUi->volumeBar, SIGNAL(valueChanged(int)), this, SIGNAL(volumeChanged(int)));
	QObject::connect(mUi->muteButton, SIGNAL(clicked(bool)), this, SLOT(mute(bool)));

	clear();
}

ViVolumeWidget::~ViVolumeWidget()
{
	delete mUi;
}

void ViVolumeWidget::mute(bool mute)
{
	if(mute)
	{
		emit muted();
		emit volumeChanged(0);
	}
	else
	{
		emit unmuted();
		emit volumeChanged(mUi->volumeBar->value());
	}
}

void ViVolumeWidget::unmute(bool unmute)
{
	mute(!unmute);
}

void ViVolumeWidget::clear()
{
	mUi->volumeBar->setValue(85);
	mUi->muteButton->setChecked(false);
}
