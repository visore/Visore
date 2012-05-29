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

	mUi->recordButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->recordButton->setCheckable(true);

	mUi->playButton->setIcon(ViThemeManager::image("play.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->playButton->setCheckable(true);

	mUi->pauseButton->setIcon(ViThemeManager::image("pause.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->pauseButton->setCheckable(true);

	mUi->stopButton->setIcon(ViThemeManager::image("stop.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);

	QObject::connect(mUi->recordButton, SIGNAL(clicked(bool)), this, SLOT(record(bool)));
	QObject::connect(mUi->playButton, SIGNAL(clicked(bool)), this, SLOT(play(bool)));
	QObject::connect(mUi->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause(bool)));
	QObject::connect(mUi->stopButton, SIGNAL(clicked(bool)), this, SLOT(stop(bool)));

	mUi->recordButton->disable();
	mUi->playButton->disable();
	mUi->pauseButton->disable();
	mUi->stopButton->disable();

	QObject::connect(mEngine, SIGNAL(inputChanged(ViAudio::Input)), this, SLOT(inputChanged(ViAudio::Input)));
}

ViPlaybackWidget::~ViPlaybackWidget()
{
	delete mUi;
}

void ViPlaybackWidget::inputChanged(ViAudio::Input input)
{
	stop(true);
	if(input == ViAudio::File)
	{
		mUi->recordButton->disable();
		mUi->playButton->enable();
		mUi->pauseButton->disable();
		mUi->stopButton->disable();
	}
	else if(input == ViAudio::Line)
	{
		mUi->recordButton->enable();
		mUi->playButton->disable();
		mUi->pauseButton->disable();
		mUi->stopButton->disable();
	}
}

void ViPlaybackWidget::record(bool checked)
{
	mIsRecording = checked;
	if(mIsRecording)
	{
		mEngine->startRecording();
		mUi->playButton->enable();
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
		mUi->pauseButton->enable();
		mUi->stopButton->enable();
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
	if(mIsPlaying || mIsPausing)
	{
		mEngine->stopPlayback();
	}
	play(false);
	mIsPlaying = false;
	mIsPausing = false;
	mUi->playButton->setChecked(mIsPlaying);
}
