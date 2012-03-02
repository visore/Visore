#include "viplaybackwidget.h"
#include "ui_viplaybackwidget.h"

ViPlaybackWidget::ViPlaybackWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mUi = new Ui::ViPlaybackWidget();
	mUi->setupUi(this);

	mIsRecording = false;
	mIsPlaying = false;
	mIsPausing = false;

	mUi->recordButton->setNormalIcon(ViThemeManager::image("recordnormal.png"));
	mUi->recordButton->setCheckedIcon(ViThemeManager::image("recordselected.png"));
	mUi->recordButton->setCheckable();

	mUi->playButton->setNormalIcon(ViThemeManager::image("playnormal.png"));
	mUi->playButton->setCheckedIcon(ViThemeManager::image("playselected.png"));
	mUi->playButton->setCheckable();

	mUi->pauseButton->setNormalIcon(ViThemeManager::image("pausenormal.png"));
	mUi->pauseButton->setCheckedIcon(ViThemeManager::image("pauseselected.png"));
	mUi->pauseButton->setCheckable();

	mUi->stopButton->setNormalIcon(ViThemeManager::image("stopnormal.png"));
	mUi->stopButton->setCheckedIcon(ViThemeManager::image("stopselected.png"));

	ViObject::connect(mUi->recordButton, SIGNAL(clicked(bool)), this, SLOT(record(bool)));
	ViObject::connect(mUi->playButton, SIGNAL(clicked(bool)), this, SLOT(play(bool)));
	ViObject::connect(mUi->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause(bool)));
	ViObject::connect(mUi->stopButton, SIGNAL(clicked(bool)), this, SLOT(stop(bool)));
}

ViPlaybackWidget::~ViPlaybackWidget()
{
	delete mUi;
}

void ViPlaybackWidget::record(bool checked)
{
	mIsRecording = checked;
	if(mIsRecording)
	{
		mEngine->startRecording();
	}
	else
	{
		mEngine->stopRecording();
	}
}

void ViPlaybackWidget::play(bool checked)
{
	mIsPlaying = checked;
	if(checked)
	{
		mEngine->startPlayback();
	}
	mUi->pauseButton->setChecked(false);
}

void ViPlaybackWidget::pause(bool checked)
{
	mIsPausing = checked;
	mIsPlaying = !mIsPausing;
	if(checked)
	{
		mUi->playButton->setChecked(false);
		mEngine->pausePlayback();
	}
	else
	{
		mUi->playButton->setChecked(mIsPlaying);
		play(true);
	}
	mUi->pauseButton->setChecked(mIsPausing);
}

void ViPlaybackWidget::stop(bool checked)
{
	mEngine->stopPlayback();
	play(false);
	mUi->playButton->setChecked(mIsPlaying);
}
