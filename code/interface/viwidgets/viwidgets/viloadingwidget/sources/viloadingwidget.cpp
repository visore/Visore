#include "viloadingwidget.h"
#include "ui_viloadingwidget.h"

ViLoadingWidget::ViLoadingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViLoadingWidget();
	mUi->setupUi(this);

	setStyleSheet("QWidget#ViLoadingWidget{ background-color: rgba(0, 0, 0, 200); }");

	QString color1 = ViThemeManager::color(15).name();
	QString color2 = ViThemeManager::color(12).name();
	QString color3 = ViThemeManager::color(11).name();
	mUi->progressBar->setStyleSheet("\
		QProgressBar\
		{\
			border: 1px solid " + color3 + ";\
			text-align: center;\
			padding: 1px;\
			border-radius: 5px;\
			background: QLinearGradient(x1: 0, x2: 0, y2: 1, y1: 0, stop: 0 #555, stop: 1 #ccc);\
		}\
		\
		QProgressBar::chunk\
		{\
			background: QLinearGradient(x1: 0, x2: 0, y2: 1, y1: 0, stop: 0 " + color1 + ", stop: 1 " + color2 + ");\
			border-radius: 5px;\
			border: 1px solid " + color3 + ";\
		}\
	");

	mTimer = new QTimer(this);
	ViObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(displayNextImage()));
}

ViLoadingWidget::~ViLoadingWidget()
{
	delete mTimer;
	delete mUi;
}

void ViLoadingWidget::setVisible(bool visible)
{
	if(visible)
	{
		start();
	}
	else
	{
		stop();
	}
	ViWidget::setVisible(visible);
}

void ViLoadingWidget::start()
{
	mCurrentImage = 0;
	displayNextImage();
	mTimer->start(100);
}

void ViLoadingWidget::stop()
{
	mTimer->stop();
}

void ViLoadingWidget::displayNextImage()
{
	++mCurrentImage;
	if(mCurrentImage > 36)
	{
		mCurrentImage = 1;
	}
	mUi->animationWidget->setStyleSheet("background: url(/home/visore/Desktop/anim/" + QString::number(mCurrentImage) + ".png) no-repeat;");
	repaint();
}
