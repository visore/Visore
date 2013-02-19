#include "viprogressbar.h"
#include "ui_viprogressbar.h"

ViProgressBar::ViProgressBar(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProgressBar();
	mUi->setupUi(this);

	QString color1 = ViThemeManager::color(ViThemeColors::MainColor6).name();
	QString color2 = ViThemeManager::color(ViThemeColors::MainColor3).name();
	QString color3 = ViThemeManager::color(ViThemeColors::BorderColor2).name();
	setStyleSheet("\
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
	setProgressStyle(ViProgressBar::Finite);
}

ViProgressBar::~ViProgressBar()
{
	delete mUi;
}

void ViProgressBar::setStyleSheet(QString styleSheet)
{
	mUi->progressBar->setStyleSheet(styleSheet);
}

void ViProgressBar::setAlignment(Qt::Alignment alignment)
{
	mUi->progressBar->setAlignment(alignment);
}

void ViProgressBar::setValue(int value)
{
	mUi->progressBar->setValue(value);
}

void ViProgressBar::setText(QString text)
{
	mText = text;
	mUi->progressBar->setFormat(text);
	setTextStyle(ViProgressBar::Text);
}

void ViProgressBar::setProgressStyle(ViProgressBar::ProgressStyle style)
{
	if(style == ViProgressBar::Finite)
	{
		mUi->progressBar->setRange(0, 100);
	}
	else if(style == ViProgressBar::Infinite)
	{
		mUi->progressBar->setRange(0, 0);
	}
}

Qt::Alignment ViProgressBar::alignment()
{
	return mUi->progressBar->alignment();
}
		
int ViProgressBar::value()
{
	return mUi->progressBar->value();
}

void ViProgressBar::setTextStyle(ViProgressBar::TextStyle style)
{
	if(style == ViProgressBar::None)
	{
		mUi->progressBar->setFormat("");
	}
	else if(style == ViProgressBar::Text)
	{
		mUi->progressBar->setFormat(mText);
	}
	else
	{
		mUi->progressBar->setFormat("%p%");
	}
}
