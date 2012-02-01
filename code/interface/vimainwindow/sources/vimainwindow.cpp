#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);

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
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
}

void ViMainWindow::setEngine(ViAudioEngine *engine)
{
	mEngine = engine;
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
		mUi->recordButton->setIcon(QIcon(":/vimainwindow/icons/recordselected.png"));
	}
	else
	{
		mIsRecording = false;
		mUi->recordButton->setIcon(QIcon(":/vimainwindow/icons/recordnormal.png"));
	}
}

void ViMainWindow::setPlaying(bool active)
{
	if(active)
	{
		mIsPlaying = true;
		mUi->playButton->setIcon(QIcon(":/vimainwindow/icons/playselected.png"));
	}
	else
	{
		mIsPlaying = false;
		mUi->playButton->setIcon(QIcon(":/vimainwindow/icons/playnormal.png"));
	}
}

void ViMainWindow::setPausing(bool active)
{
	if(active)
	{
		mIsPaused = true;
		mUi->pauseButton->setIcon(QIcon(":/vimainwindow/icons/pauseselected.png"));
	}
	else
	{
		mIsPaused = false;
		mUi->pauseButton->setIcon(QIcon(":/vimainwindow/icons/pausenormal.png"));
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

QMainWindow* createObject(ViAudioEngine *engine)
{
	ViMainWindow *window = new ViMainWindow();
	window->setEngine(engine);
	return window;
}

#ifdef __cplusplus
}
#endif
