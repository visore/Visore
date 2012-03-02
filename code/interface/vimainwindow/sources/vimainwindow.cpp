#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(ViAudioEngine *engine, QWidget *parent)
	: QMainWindow(parent)
{
	mEngine = engine;

	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mUi->centralwidget->setStyleSheet("QWidget#centralwidget{ background-image: url(" + ViThemeManager::background("tile.png") + "); }");

	//Playback widget
	mPlaybackWidget = new ViPlaybackWidget(mEngine, mUi->playbackContainer);
	mUi->playbackContainer->layout()->addWidget(mPlaybackWidget);

	//Wave forms
	mOriginalWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Original, mUi->originalWaveContainer);
	mUi->originalWaveContainer->layout()->addWidget(mOriginalWaveWidget);
	mCorrectedWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Corrected, mUi->correctedWaveContainer);
	mUi->correctedWaveContainer->layout()->addWidget(mCorrectedWaveWidget);

	//Volume widget
	//mVolumeWidget = new ViVolumeDialWidget(mEngine, mUi->volumeContainer);
	//mUi->volumeContainer->layout()->addWidget(mVolumeWidget);
	mVolumeWidget = new ViVolumeBarWidget(mEngine, mUi->volumeContainer);
	mUi->volumeContainer->layout()->addWidget(mVolumeWidget);

	QObject::connect(mUi->lineInput, SIGNAL(clicked()), this, SLOT(selectLineInput()));
	QObject::connect(mUi->fileInput, SIGNAL(clicked()), this, SLOT(selectFileInput()));
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
	delete mPlaybackWidget;
	delete mOriginalWaveWidget;
	delete mCorrectedWaveWidget;
	delete mVolumeWidget;
}

void ViMainWindow::selectLineInput()
{
}

void ViMainWindow::selectFileInput()
{
	mEngine->setInputFilePath("/home/visore/Desktop/a.wav");
}
