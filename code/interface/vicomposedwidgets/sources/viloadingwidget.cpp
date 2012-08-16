#include "viloadingwidget.h"
#include "ui_viloadingwidget.h"
#include "vimainwindow.h"

ViLoadingWidget::ViLoadingWidget()
	: ViWidget(ViMainWindow::instance())
{
	mUi = new Ui::ViLoadingWidget();
	mUi->setupUi(this);

	setStyleSheet("ViLoadingWidget{background-color: rgba(0, 0, 0, 200);}");

	mUi->cancelButton->setSize(50, 20);
	mUi->cancelButton->setGlow(ViThemeManager::color(14));
	//mUi->cancelButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);

	QObject::connect(engine(), SIGNAL(progressStarted()), this, SLOT(start()));
	QObject::connect(engine(), SIGNAL(statusChanged(QString)), this, SLOT(setText(QString)));
	QObject::connect(engine(), SIGNAL(progressFinished()), this, SLOT(stop()));

	stop();
}

ViLoadingWidget::~ViLoadingWidget()
{
	delete mUi;
}

ViLoadingWidget& ViLoadingWidget::instance()
{
	static ViLoadingWidget widget;
	return widget;
}

void ViLoadingWidget::start()
{
	start(true, false, "", ViProgressBar::None, ViProgressBar::Finite);
}

void ViLoadingWidget::start(bool animation, bool button, QString text, ViProgressBar::TextStyle textStyle, ViProgressBar::ProgressStyle progressStyle)
{
	showAnimation(animation);
	showButton(button);
	setText(text);
	setTextStyle(textStyle);
	setProgressStyle(progressStyle);

	startAnimation();
	show();
}

void ViLoadingWidget::stop()
{
	stopAnimation();
	hide();
}

void ViLoadingWidget::progress(short percentage)
{
	mUi->progressBar->setValue(percentage);
}

void ViLoadingWidget::showAnimation(bool show)
{
	mHasAnimation = show;
	mUi->animationWidget->setVisible(mHasAnimation);
	if(show)
	{
		QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(displayNextImage()));
	}
	else
	{
		stopAnimation();
		QObject::disconnect(&mTimer, SIGNAL(timeout()), this, SLOT(displayNextImage()));
	}
}

void ViLoadingWidget::showButton(bool show)
{
	mUi->cancelButton->setVisible(show);
}

void ViLoadingWidget::setTextStyle(ViProgressBar::TextStyle style)
{
	mUi->progressBar->setTextStyle(style);
}

void ViLoadingWidget::setProgressStyle(ViProgressBar::ProgressStyle style)
{
	mUi->progressBar->setProgressStyle(style);
}

void ViLoadingWidget::setText(QString text)
{
	mUi->progressBar->setTextStyle(ViProgressBar::Text);
	mUi->progressBar->setText(text);
}

void ViLoadingWidget::startAnimation()
{
	if(mHasAnimation)
	{
		mCurrentImage = 0;
		displayNextImage();
		mTimer.start(100);
	}
}

void ViLoadingWidget::stopAnimation()
{
	mTimer.stop();
}

void ViLoadingWidget::displayNextImage()
{
	++mCurrentImage;
	if(mCurrentImage > 36)
	{
		mCurrentImage = 1;
	}
	mUi->animationWidget->setStyleSheet("background: url(" + ViThemeManager::image("loading/" + QString::number(mCurrentImage) + ".png", ViThemeImage::Normal, ViThemeManager::Animation).path() + ") no-repeat;");
	repaint();
}
