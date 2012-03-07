#include "viplaybackwidget.h"
#include "ui_viplaybackwidget.h"

ViPlaybackWidget::ViPlaybackWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViPlaybackWidget();
	mUi->setupUi(this);

	mIsRecording = false;
	mIsPlaying = false;
	mIsPausing = false;

	mUi->recordButton->setGlow(ViThemeManager::color(14));
	mUi->playButton->setGlow(ViThemeManager::color(14));
	mUi->pauseButton->setGlow(ViThemeManager::color(14));
	mUi->stopButton->setGlow(ViThemeManager::color(14));

	mUi->recordButton->setNormalIcon(ViThemeManager::image("record.png", ViThemeManager::Normal));
	mUi->recordButton->setSelectedIcon(ViThemeManager::image("record.png", ViThemeManager::Selected));
	mUi->recordButton->setCheckable();

	mUi->playButton->setNormalIcon(ViThemeManager::image("play.png", ViThemeManager::Normal));
	mUi->playButton->setSelectedIcon(ViThemeManager::image("play.png", ViThemeManager::Selected));
	mUi->playButton->setCheckable();

	mUi->pauseButton->setNormalIcon(ViThemeManager::image("pause.png", ViThemeManager::Normal));
	mUi->pauseButton->setSelectedIcon(ViThemeManager::image("pause.png", ViThemeManager::Selected));
	mUi->pauseButton->setCheckable();

	mUi->stopButton->setNormalIcon(ViThemeManager::image("stop.png", ViThemeManager::Normal));
	mUi->stopButton->setSelectedIcon(ViThemeManager::image("stop.png", ViThemeManager::Selected));

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
	if(!mIsPlaying)
	{
		mIsPlaying = checked;
	}
	else
	{
		mUi->playButton->setChecked(true);
	}
	if(checked)
	{
		mEngine->startPlayback();
	}
	mUi->pauseButton->setChecked(false);
}

void ViPlaybackWidget::pause(bool checked)
{
	if(mIsPlaying || mIsPausing)
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
			mEngine->startPlayback();
		}
	}
	mUi->pauseButton->setChecked(mIsPausing);
}

void ViPlaybackWidget::stop(bool checked)
{
	mEngine->stopPlayback();
	play(false);
	mIsPlaying = false;
	mIsPausing = false;
	mUi->playButton->setChecked(mIsPlaying);
}
