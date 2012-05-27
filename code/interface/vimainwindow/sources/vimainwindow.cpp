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
	delete mLoadingWidget;
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
	hideLoading();
}
