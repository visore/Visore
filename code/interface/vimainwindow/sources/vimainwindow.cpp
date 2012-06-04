#include "vimainwindow.h"
#include "ui_vimainwindow.h"
#include <iostream>
using namespace std;
ViMainWindow *ViMainWindow::mWindow = NULL;

ViMainWindow::ViMainWindow()
	: QMainWindow()
{
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
	//delete mOriginalWaveWidget;
	//delete mCorrectedWaveWidget;
	delete mLoadingWidget;
	//delete mSongInfoWidget;
}

ViMainWindow* ViMainWindow::instance()
{
	if(mWindow == NULL)
	{
		mWindow = new ViMainWindow();
		mWindow->initialize();
	}
	return mWindow;
}

void ViMainWindow::progress(qreal percentage)
{
	mLoadingWidget->progress(percentage);
}

void ViMainWindow::hideLoading()
{
	mLoadingWidget->setVisible(false);
}

void ViMainWindow::showLoading(bool animation, bool button, ViLoadingWidget::TextStyle style, QString text)
{
	mLoadingWidget->showAnimation(animation);
	mLoadingWidget->showButton(button);
	mLoadingWidget->setTextStyle(style);
	mLoadingWidget->setText(text);
	mLoadingWidget->setVisible(true);
}

void ViMainWindow::showWave()
{
	//mUi->widgetStack->setCurrentIndex(0);
}

void ViMainWindow::showSpectrum()
{
	//mUi->widgetStack->setCurrentIndex(1);
}

void ViMainWindow::resizeEvent(QResizeEvent *event)
{
	mLoadingWidget->resize(event->size());
    event->accept();
}

void ViMainWindow::initialize()
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mEngine = ViAudioEngine::instance();

	mLoadingWidget = new ViLoadingWidget(centralWidget());

	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile.png", ViThemeImage::Normal, ViThemeManager::Background).path() + ") repeat-x;}");

/*
	//Wave forms
	mOriginalWaveWidget = new ViWaveFormWidget(ViAudioBuffer::Original, mUi->originalWaveContainer);
	mUi->originalWaveContainer->layout()->addWidget(mOriginalWaveWidget);
	mCorrectedWaveWidget = new ViWaveFormWidget(ViAudioBuffer::Corrected, mUi->correctedWaveContainer);
	mUi->correctedWaveContainer->layout()->addWidget(mCorrectedWaveWidget);
	ViObject::connect(mOriginalWaveWidget, SIGNAL(pointerMoved(qint32)), mCorrectedWaveWidget, SLOT(setPointer(qint32)));
	ViObject::connect(mCorrectedWaveWidget, SIGNAL(pointerMoved(qint32)), mOriginalWaveWidget, SLOT(setPointer(qint32)));

	//Song info
	mSongInfoWidget = new ViSongInfoWidget(mUi->songInfoContainer);
	mUi->songInfoContainer->layout()->addWidget(mSongInfoWidget);

	showWave();*/

	hideLoading();
}
