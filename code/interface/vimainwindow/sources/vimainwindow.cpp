#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(ViAudioEngine *engine, QWidget *parent)
	: QMainWindow(parent)
{
	mEngine = engine;

	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mUi->centralwidget->setStyleSheet("QWidget#centralwidget{ background-image: url(" + ViThemeManager::background("tile.png") + "); }");

	//Wave forms
	mOriginalWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Original, mUi->originalWaveContainer);
	mUi->originalWaveContainer->layout()->addWidget(mOriginalWaveWidget);
	mCorrectedWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Corrected, mUi->correctedWaveContainer);
	mUi->correctedWaveContainer->layout()->addWidget(mCorrectedWaveWidget);

	//Volume widget
	mVolumeWidget = new ViVolumeWidget(mEngine, mUi->volumeContainer);
	mUi->volumeContainer->layout()->addWidget(mVolumeWidget);

	QObject::connect(mUi->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(mUi->recordButton, SIGNAL(clicked()), this, SLOT(record()));
	QObject::connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
	QObject::connect(mUi->playButton, SIGNAL(clicked()), this, SLOT(play()));
	QObject::connect(mUi->pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
	QObject::connect(mUi->stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	QObject::connect(mUi->lineInput, SIGNAL(clicked()), this, SLOT(selectLineInput()));
	QObject::connect(mUi->fileInput, SIGNAL(clicked()), this, SLOT(selectFileInput()));

	mIsRecording = false;
	mIsPlaying = false;
	mIsPaused = false;
	setRecording(mIsRecording);
	setPlaying(mIsPlaying);
	setPausing(mIsPaused);
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
	delete mOriginalWaveWidget;
	delete mCorrectedWaveWidget;
	delete mVolumeWidget;
}

void ViMainWindow::reset()
{
	stop();
	setRecording(false);
	mEngine->reset();
}

void ViMainWindow::record()
{
	if(!mIsRecording)
	{
		mEngine->startRecording();
	}
	else
	{
		mEngine->stopRecording();
	}	
	setRecording(!mIsRecording);
}

void ViMainWindow::save()
{
	mEngine->setOutputFilePath("/home/visore/Desktop/visore.mp3");
	mEngine->startOutputFile();
}

void ViMainWindow::play()
{
	if(!mIsPlaying)
	{
		mEngine->startPlayback();
		setPausing(false);
		setPlaying(!mIsPlaying);
	}
}

void ViMainWindow::pause()
{
	if(!mIsPaused)
	{
		mEngine->pausePlayback();
		setPlaying(false);
		setPausing(!mIsPaused);
	}
}

void ViMainWindow::stop()
{
	mEngine->stopPlayback();
	setPausing(false);
	setPlaying(false);
}

void ViMainWindow::selectLineInput()
{
	mUi->recordButton->setEnabled(true);
}

void ViMainWindow::selectFileInput()
{
	mUi->recordButton->setEnabled(false);
	mEngine->setInputFilePath("/home/visore/Desktop/a.wav");
}

void ViMainWindow::setRecording(bool active)
{
	if(active)
	{
		mIsRecording = true;
		mUi->recordButton->setIcon(ViThemeManager::icon("recordselected.png"));
	}
	else
	{
		mIsRecording = false;
		mUi->recordButton->setIcon(ViThemeManager::icon("recordnormal.png"));
	}
}

void ViMainWindow::setPlaying(bool active)
{
	if(active)
	{
		mIsPlaying = true;
		mUi->playButton->setIcon(ViThemeManager::icon("playselected.png"));
	}
	else
	{
		mIsPlaying = false;
		mUi->playButton->setIcon(ViThemeManager::icon("playnormal.png"));
	}
}

void ViMainWindow::setPausing(bool active)
{
	if(active)
	{
		mIsPaused = true;
		mUi->pauseButton->setIcon(ViThemeManager::icon("pauseselected.png"));
	}
	else
	{
		mIsPaused = false;
		mUi->pauseButton->setIcon(ViThemeManager::icon("pausenormal.png"));
	}
}
