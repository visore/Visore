#include "viloadingwidget.h"
#include "ui_viloadingwidget.h"

ViLoadingWidget::ViLoadingWidget(QWidget *parent, bool animation, bool button, ViLoadingWidget::TextStyle textStyle, ViProgressBar::ProgressStyle progressStyle)
	: ViWidget(parent)
{
	mUi = new Ui::ViLoadingWidget();
	mUi->setupUi(this);

	setStyleSheet("QWidget#ViLoadingWidget{background-color: rgba(0, 0, 0, 200);}");

	mTimer = new QTimer(this);

	mUi->cancelButton->setSize(500,200);
	mUi->cancelButton->setGlow(ViThemeManager::color(14));
	mUi->cancelButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);

	showAnimation(animation);
	showButton(button);
	setTextStyle(textStyle);
	setProgressStyle(progressStyle);
	progress(0);
}

ViLoadingWidget::~ViLoadingWidget()
{
	if(mTimer != NULL)
	{
		delete mTimer;
		mTimer = NULL;
	}
	delete mUi;
}

void ViLoadingWidget::progress(short percentage)
{
	mUi->progressBar->setValue(percentage);
}

void ViLoadingWidget::showAnimation(bool show)
{
	mHasAnimation = show;
	mUi->animationWidget->setVisible(mHasAnimation);
	if(mHasAnimation)
	{
		QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(displayNextImage()));
	}
	else
	{
		stop();
		QObject::disconnect(mTimer, SIGNAL(timeout()), this, SLOT(displayNextImage()));
	}
}

void ViLoadingWidget::showButton(bool show)
{
	mUi->cancelButton->setVisible(show);
}

void ViLoadingWidget::setTextStyle(ViLoadingWidget::TextStyle style)
{
	mTextStyle = style;
	setText();
}

void ViLoadingWidget::setProgressStyle(ViProgressBar::ProgressStyle style)
{
	mUi->progressBar->setProgressStyle(style);
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

void ViLoadingWidget::setText(QString text)
{
	if(mTextStyle == ViLoadingWidget::Text || mTextStyle == ViLoadingWidget::None)
	{
		mUi->progressBar->setText(text);
	}
}

void ViLoadingWidget::start()
{
	if(mHasAnimation)
	{
		mCurrentImage = 0;
		displayNextImage();
		mTimer->start(100);
	}
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
	mUi->animationWidget->setStyleSheet("background: url(" + ViThemeManager::image("loading/" + QString::number(mCurrentImage) + ".png", ViThemeImage::Normal, ViThemeManager::Animation).path() + ") no-repeat;");
	repaint();
}
