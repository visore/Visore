#include "vicontentwidget.h"
#include "ui_vicontentwidget.h"

ViContentWidget::ViContentWidget(QWidget *parent)
	: QWidget(parent)
{
	mUi = new Ui::ViContentWidget();
	mUi->setupUi(this);
}

ViContentWidget::~ViContentWidget()
{
	delete mUi;
	delete mPlaybackWidget;
	delete mOriginalWaveWidget;
	delete mCorrectedWaveWidget;
	delete mVolumeWidget;
}

void ViContentWidget::initialize(ViAudioEngine *engine)
{
	mEngine = engine;

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

void ViContentWidget::selectLineInput()
{
}

void ViContentWidget::selectFileInput()
{
	mEngine->setInputFilePath("/home/visore/Desktop/a.wav");
}
