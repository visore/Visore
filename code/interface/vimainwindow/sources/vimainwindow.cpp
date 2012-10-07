#include "vimainwindow.h"
#include "ui_vimainwindow.h"

#include "vifilebrowser.h"

#include "viinfowidget.h"

ViMainWindow* ViMainWindow::mWindow = NULL;

ViMainWindow::ViMainWindow()
	: QMainWindow()
{
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
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

void ViMainWindow::show()
{
	if(ViManager::isWindowFullScreen())
	{
		showFullScreen();
	}
	else if(ViManager::isWindowMaximized())
	{
		showMaximized();
	}
	else
	{
		resize(ViManager::windowSize());
		move(ViManager::windowPosition());
		QMainWindow::show();
	}
}

void ViMainWindow::resizeEvent(QResizeEvent *event)
{
	ViLoadingWidget::instance().resize(event->size());
    event->accept();
	ViManager::setWindowSize(event->size());
}

void ViMainWindow::moveEvent(QMoveEvent *event)
{
	ViManager::setWindowPosition(event->pos());
}

void ViMainWindow::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::WindowStateChange)
	{
		ViManager::setWindowFullScreen(isFullScreen());
		ViManager::setWindowMaximized(isMaximized());
	}
}

void ViMainWindow::initialize()
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mEngine = ViAudioEngine::instance();

	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile.png", ViThemeImage::Normal, ViThemeManager::Background).path() + ") repeat-x;}");

	//ViLoadingWidget::stop();

	ViInfoWidget *w = new ViInfoWidget();
	w->addEntry("asrthvcbmldsgnlkmdngcfm,nsd");
	w->addEntry("asrt", "sdfsdfdsfsdfdsf***************");
	w->addEntry("asrt999999999999", "sdfsdfdsfsdfdsf");
	w->show();
}
