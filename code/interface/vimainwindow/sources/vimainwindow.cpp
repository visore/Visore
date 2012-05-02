#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(ViAudioEngine *engine, QWidget *parent)
	: QMainWindow(parent)
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mEngine = engine;
	initialize();
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
	delete mOriginalWaveWidget;
	delete mCorrectedWaveWidget;
	delete mInputContainer;
	delete mOutputContainer;
	delete mControlContainer;
	delete mLoadingWidget;
}

void ViMainWindow::resizeEvent(QResizeEvent *event)
{
	mLoadingWidget->resize(event->size());
    event->accept();
}

void ViMainWindow::setLoading(bool load)
{
	mLoadingWidget->setVisible(load);
}

void ViMainWindow::initialize()
{
	mLoadingWidget = new ViLoadingWidget(centralWidget());
	setLoading(false);

	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile.png", ViThemeImage::Normal, ViThemeManager::Background).path() + ") repeat-x;}");

	//Wave forms
	mOriginalWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Original, mUi->originalWaveContainer);
	mUi->originalWaveContainer->layout()->addWidget(mOriginalWaveWidget);
	mCorrectedWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Corrected, mUi->correctedWaveContainer);
	mUi->correctedWaveContainer->layout()->addWidget(mCorrectedWaveWidget);
	ViObject::connect(mOriginalWaveWidget, SIGNAL(pointerMoved(qint32)), mCorrectedWaveWidget, SLOT(setPointer(qint32)));
	ViObject::connect(mCorrectedWaveWidget, SIGNAL(pointerMoved(qint32)), mOriginalWaveWidget, SLOT(setPointer(qint32)));

	mUi->tabWidget->setHeight(120);
	mUi->tabWidget->setRounding(0, 5);
	mUi->tabWidget->setTabOffset(5);

	mInputContainer = new ViInputContainer(mUi->tabWidget);
	mInputContainer->setEngine(mEngine);
	mUi->tabWidget->addTab("Input", mInputContainer);

	mControlContainer = new ViControlContainer(mUi->tabWidget);
	mControlContainer->setEngine(mEngine);
	mUi->tabWidget->addTab("Control", mControlContainer);

	mOutputContainer = new ViOutputContainer(mUi->tabWidget);
	mOutputContainer->setEngine(mEngine);
	mUi->tabWidget->addTab("Output", mOutputContainer);
}
