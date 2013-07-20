#include <vimainwindow.h>
#include <ui_vimainwindow.h>
#include <vistackedwidget.h>
#include <vimainmenu.h>
#include <viloadingwidget.h>
#include <viaboutdialog.h>

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

	centralWidget()->setStyleSheet(ViThemeManager::globalStyleSheet());

	//setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile").path() + ") repeat-x;}");
	QString color = ViThemeManager::color(ViThemeColors::MainColor6).name();
	mUi->lineWidget->setStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0.0 transparent, stop: 0.02 " + color + ", stop: 0.7 " + color + ", stop: 1.0 transparent);");

    mUi->stack->layout()->addWidget(ViStackedWidget::widget());
    mUi->stack->setStyleSheet(".QWidget{background:transparent;border:0px;}");

	mUi->aboutButton->disableBackground();
	mUi->aboutButton->disableBorder();
	mUi->aboutButton->setIcon(ViThemeManager::icon("about"), 24);
	QObject::connect(mUi->aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

	int index = ViStackedWidget::addWidget(new ViMainMenu(), false);
	ViStackedWidget::setCurrentIndex(index);
	mUi->logoButton->setProperty("index", index);
	QObject::connect(mUi->logoButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->logoButton->disableBackground();
	mUi->logoButton->disableBorder();
	mUi->logoButton->addStyleSheet("background: qradialgradient(cx: 0.5, cy: 0.5, radius: 1, fx: 0.5, fy: 0.5, stop: 0 " + ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name() + ", stop: 0.45 transparent);", ViButton::Hovered);
	mUi->logoButton->setSize(64, 64);
	mUi->logoButton->setIcon(ViThemeManager::icon("logo"), 64);

	ViFont font = ViThemeManager::font(ViThemeFonts::MainFont);
	font.setPointSize(30);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	mUi->logoLabel->setFont(font);
    mUi->logoLabel->setStyleSheet("color: " + ViThemeManager::color(ViThemeColors::MainColor6).name());
}

void ViMainWindow::showAbout()
{
	ViAboutDialog dialog(this);
	dialog.exec();
}
