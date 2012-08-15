#include "viratingwidget.h"
#include "vithememanager.h"
#include <QPainter>

#define NUMBER_OF_STARS 10

inline int GRAY(int red, int green, int blue)
{
	return (red * 4 + green * 4 + blue * 4) / 32;
}

inline int GRAY(QRgb rgb)
{
	return GRAY(qRed(rgb), qGreen(rgb), qBlue(rgb));
}

ViRatingItem::ViRatingItem(QWidget *parent, bool grayScale)
	: ViWidget(parent)
{
	mImage.load(ViThemeManager::image("star.png", ViThemeImage::Normal, ViThemeManager::Icon).path());
	if(grayScale)
	{
		int value;
		QRgb rgb;
		for(int i = 0; i < mImage.width(); ++i)	
		{
			for(int j = 0; j < mImage.height(); ++j)
			{
				rgb = mImage.pixel(i, j);
				value = GRAY(rgb);
				mImage.setPixel(i, j, qRgba(value, value, value, qAlpha(rgb)));
			}
		}
	}
	mOffset = 0;
}

int ViRatingItem::imageWidth()
{
	return mImage.width();
}

void ViRatingItem::setOffset(int offset)
{
	mOffset = offset;
}

void ViRatingItem::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawImage(-mOffset, 0, mImage);
}

ViRatingWidget::ViRatingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mNormalWidget = new QWidget(this);
	mNormalLayout = new QHBoxLayout(mNormalWidget);
	mNormalLayout->setContentsMargins(0, 0, 0, 0);
	mNormalLayout->setSpacing(0);
	for(int i = 0; i < NUMBER_OF_STARS; ++i)
	{
		ViRatingItem *star = new ViRatingItem(this);
		mNormalStars.append(star);
		mNormalLayout->addWidget(star);
	}

	mGrayWidget = new QWidget(this);
	mGrayLayout = new QHBoxLayout(mGrayWidget);
	mGrayLayout->setContentsMargins(0, 0, 0, 0);
	mGrayLayout->setSpacing(0);
	for(int i = 0; i < NUMBER_OF_STARS; ++i)
	{
		ViRatingItem *star = new ViRatingItem(this, true);
		mGrayStars.append(star);
		mGrayLayout->addWidget(star);
	}

	mSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	mLayout = new QHBoxLayout(this);
	mLayout->setContentsMargins(0, 0, 0, 0);
	mLayout->setSpacing(0);
	mLayout->addWidget(mNormalWidget);
	mLayout->addWidget(mGrayWidget);
	mLayout->addSpacerItem(mSpacer);

	setRating(0);
}

ViRatingWidget::~ViRatingWidget()
{
	delete mLayout;
	delete mNormalLayout;
	delete mGrayLayout;
	delete mNormalWidget;
	delete mGrayWidget;
}

void ViRatingWidget::setRating(qint16 percentage)
{
	static int width = mNormalStars[0]->imageWidth();
	qfloat ratio = NUMBER_OF_STARS * (percentage / 100.0);
	qint16 fullStars = ratio;
	qfloat reduction = width * (ratio - fullStars);
	for(int i = 0; i < NUMBER_OF_STARS; ++i)
	{
		mGrayStars[i]->setOffset(0);
		if(i < fullStars)
		{
			mNormalStars[i]->setFixedWidth(width);
			mGrayStars[i]->setFixedWidth(0);
		}
		else if(i > fullStars || reduction == 0)
		{
			mNormalStars[i]->setFixedWidth(0);
			mGrayStars[i]->setFixedWidth(width);
		}
		else
		{
			mNormalStars[i]->setFixedWidth(reduction);
			mGrayStars[i]->setOffset(reduction);
			mGrayStars[i]->setFixedWidth(width - reduction);
		}
	}
}
