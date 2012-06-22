#include "vimainwindow.h"
#include "ui_vimainwindow.h"

#include "vifilebrowser.h"

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

void ViMainWindow::progress(short percentage)
{
	mLoadingWidget->progress(percentage);
}

void ViMainWindow::hideLoading()
{
	mLoadingWidget->setVisible(false);
}

void ViMainWindow::showLoading(bool animation, bool button, ViLoadingWidget::TextStyle textStyle, QString text, ViProgressBar::ProgressStyle progressStyle)
{
	mLoadingWidget->showAnimation(animation);
	mLoadingWidget->showButton(button);
	mLoadingWidget->setTextStyle(textStyle);
	mLoadingWidget->setProgressStyle(progressStyle);
	mLoadingWidget->setText(text);
	mLoadingWidget->setVisible(true);
}

void ViMainWindow::buffering(short bufferingProgress)
{
	if(!mBufferingStarted)
	{
		mBufferingStarted = true;
		showLoading(true, false, ViLoadingWidget::Text, "Buffering");
	}
	progress(bufferingProgress);
	if(bufferingProgress >= 100)
	{
		mBufferingStarted = false;
		hideLoading();
	}
}

void ViMainWindow::loadProject()
{
	showLoading(true, false, ViLoadingWidget::Text, "Loading Project", ViProgressBar::Infinite);
}

void ViMainWindow::saveProject()
{
	showLoading(true, false, ViLoadingWidget::Text, "Saving Project", ViProgressBar::Infinite);
}

void ViMainWindow::resizeEvent(QResizeEvent *event)
{
	mLoadingWidget->resize(event->size());
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

	mLoadingWidget = new ViLoadingWidget(centralWidget());
	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile.png", ViThemeImage::Normal, ViThemeManager::Background).path() + ") repeat-x;}");
	hideLoading();

	mBufferingStarted = false;
	QObject::connect(mEngine, SIGNAL(buffering(short)), this, SLOT(buffering(short)));

	QObject::connect(mEngine, SIGNAL(loadProjectStarted()), this, SLOT(loadProject()));
	QObject::connect(mEngine, SIGNAL(saveProjectStarted()), this, SLOT(saveProject()));
	QObject::connect(mEngine, SIGNAL(projectFinished()), this, SLOT(hideLoading()));

mEngine->loadProject("/home/visore/t.vip");

ViFileBrowser *ff = new ViFileBrowser();
ff->setMode(ViFileBrowser::OpenFile);
ff->addFilter("Visore Project (*.vip)");
ff->show();
}
