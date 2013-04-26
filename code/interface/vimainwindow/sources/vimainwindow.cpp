#include <vimainwindow.h>
#include <ui_vimainwindow.h>
#include <vistackedwidget.h>
#include <vimainmenu.h>
#include <viloadingwidget.h>

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
	ViLoadingWidget::instance()->resize(event->size());
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

	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile").path() + ") repeat-x;}");

	mUi->stack->layout()->addWidget(ViStackedWidget::widget());

	int index = ViStackedWidget::addWidget(new ViMainMenu());
	ViStackedWidget::setCurrentIndex(index);
	mUi->logoButton->addFunctionCall(SIGNAL(clicked()), ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index));

	mUi->logoButton->setSize(64, 64);
	mUi->logoButton->setIcon("logo", 64);
	mUi->logoButton->disableBackground();
	mUi->logoButton->setGlow(ViGradientCreator::Circle);

	ViFont font = ViThemeManager::font(ViThemeFonts::MainFont);
	font.setPointSize(30);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	mUi->logoLabel->setFont(font);
	mUi->logoLabel->setStyleSheet("color: " + ViThemeManager::color(ViThemeColors::MainColor6).name());
}
