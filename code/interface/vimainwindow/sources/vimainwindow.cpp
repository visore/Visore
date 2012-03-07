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
	delete mInputWidget;
	delete mControlWidget;
}

void ViMainWindow::initialize()
{
	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::backgroundPath("tile.png") + ") repeat-x;}");

	//Wave forms
	mOriginalWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Original, mUi->originalWaveContainer);
	mUi->originalWaveContainer->layout()->addWidget(mOriginalWaveWidget);
	mCorrectedWaveWidget = new ViWaveFormWidget(mEngine, ViAudioBuffer::Corrected, mUi->correctedWaveContainer);
	mUi->correctedWaveContainer->layout()->addWidget(mCorrectedWaveWidget);

	mUi->tabWidget->setHeight(200);

	mInputWidget = new ViInputWidget(mUi->tabWidget);
	mInputWidget->setEngine(mEngine);
	mUi->tabWidget->addTab("Input", mInputWidget);

	mControlWidget = new ViControlWidget(mUi->tabWidget);
	mControlWidget->setEngine(mEngine);
	mUi->tabWidget->addTab("Control", mControlWidget);


	//mUi->tabWidget->addTab("Number 2");
	//mUi->tabWidget->addTab("Again");
}
