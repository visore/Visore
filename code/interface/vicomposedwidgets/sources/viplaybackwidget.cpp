#include "viplaybackwidget.h"
#include "ui_viplaybackwidget.h"

ViPlaybackWidget::ViPlaybackWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViPlaybackWidget();
	mUi->setupUi(this);

	clear();

	mUi->playButton->setCheckable(true);
	mUi->pauseButton->setCheckable(true);

	mUi->playButton->setIcon(ViThemeManager::icon("play"), 36);
	mUi->pauseButton->setIcon(ViThemeManager::icon("pause"), 36);
	mUi->stopButton->setIcon(ViThemeManager::icon("stop"), 36);

	QObject::connect(mUi->playButton, SIGNAL(clicked(bool)), this, SLOT(play()));
	QObject::connect(mUi->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause()));
	QObject::connect(mUi->stopButton, SIGNAL(clicked(bool)), this, SLOT(stop()));

	QObject::connect(mUi->volume, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
	QObject::connect(mUi->positionSlider, SIGNAL(positionMoved(ViAudioPosition)), this, SIGNAL(positionMoved(ViAudioPosition)));
}

ViPlaybackWidget::~ViPlaybackWidget()
{
	clear();
	delete mUi;
}

void ViPlaybackWidget::clear()
{
	stop();

	mUi->playButton->setChecked(false);
	mUi->pauseButton->setChecked(false);
	mUi->playButton->disable();
	mUi->pauseButton->disable();
	mUi->stopButton->disable();

	mUi->volume->clear();
	mUi->positionSlider->clear();
}

void ViPlaybackWidget::play()
{
	if(mUi->playButton->isChecked())
	{
		mUi->pauseButton->setChecked(false);
		mUi->pauseButton->enable();
		mUi->stopButton->enable();
		emit played();
	}
}

void ViPlaybackWidget::pause()
{
	if(mUi->pauseButton->isChecked())
	{
		mUi->playButton->setChecked(false);
		emit paused();
	}
	else
	{
		mUi->playButton->setChecked(true);
		emit played();
	}
}

void ViPlaybackWidget::stop()
{
	mUi->playButton->setChecked(false);
	mUi->pauseButton->setChecked(false);
	mUi->playButton->enable();
	mUi->pauseButton->disable();
	mUi->stopButton->disable();
	mUi->positionSlider->changePosition(ViAudioPosition());
	emit stopped();
}

void ViPlaybackWidget::changeVolume(int volume)
{
	mUi->volume->changeVolume(volume);
}

void ViPlaybackWidget::changePosition(ViAudioPosition position)
{
	if(position.isValid())
	{
		mUi->positionSlider->changePosition(position);
	}
	else
	{
		stop();
	}
}

void ViPlaybackWidget::changeDuration(ViAudioPosition duration)
{
	if(duration.samples() > 0)
	{
		mUi->playButton->enable();
	}
	mUi->positionSlider->changeDuration(duration);
}
