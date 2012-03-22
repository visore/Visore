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

	mUi->recordButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->recordButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->recordButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Disabled, ViThemeManager::Icon), ViThemeImage::Disabled);
	mUi->recordButton->setCheckable();

	mUi->playButton->setIcon(ViThemeManager::image("play.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->playButton->setIcon(ViThemeManager::image("play.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->playButton->setIcon(ViThemeManager::image("play.png", ViThemeImage::Disabled, ViThemeManager::Icon), ViThemeImage::Disabled);
	mUi->playButton->setCheckable();

	mUi->pauseButton->setIcon(ViThemeManager::image("pause.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->pauseButton->setIcon(ViThemeManager::image("pause.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->pauseButton->setIcon(ViThemeManager::image("pause.png", ViThemeImage::Disabled, ViThemeManager::Icon), ViThemeImage::Disabled);
	mUi->pauseButton->setCheckable();

	mUi->stopButton->setIcon(ViThemeManager::image("stop.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mUi->stopButton->setIcon(ViThemeManager::image("stop.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Selected);
	mUi->stopButton->setIcon(ViThemeManager::image("stop.png", ViThemeImage::Disabled, ViThemeManager::Icon), ViThemeImage::Disabled);

	ViObject::connect(mUi->recordButton, SIGNAL(clicked(bool)), this, SLOT(record(bool)));
	ViObject::connect(mUi->playButton, SIGNAL(clicked(bool)), this, SLOT(play(bool)));
	ViObject::connect(mUi->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause(bool)));
	ViObject::connect(mUi->stopButton, SIGNAL(clicked(bool)), this, SLOT(stop(bool)));

	mUi->recordButton->disable();
	mUi->playButton->disable();
	mUi->pauseButton->disable();
	mUi->stopButton->disable();
}

ViPlaybackWidget::~ViPlaybackWidget()
{
	delete mUi;
}

void ViPlaybackWidget::setEngine(ViAudioEngine *engine)
{
	ViWidget::setEngine(engine);
	ViObject::connect(mEngine, SIGNAL(inputChanged(ViAudioEngine::ViAudioType)), this, SLOT(inputChanged(ViAudioEngine::ViAudioType)));
}

void ViPlaybackWidget::inputChanged(ViAudioEngine::ViAudioType type)
{
	stop(true);
	if(type == ViAudioEngine::File)
	{
		mUi->recordButton->disable();
		mUi->playButton->enable();
		mUi->pauseButton->disable();
		mUi->stopButton->disable();
	}
	else if(type == ViAudioEngine::Stream)
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
	mEngine->stopPlayback();
	play(false);
	mIsPlaying = false;
	mIsPausing = false;
	mUi->playButton->setChecked(mIsPlaying);
}
