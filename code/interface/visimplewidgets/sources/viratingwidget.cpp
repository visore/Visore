#include "viratingwidget.h"
#include "ui_viratingwidget.h"

#include "vithememanager.h"
#include <QtCore/qmath.h>

ViRatingWidget::ViRatingWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViRatingWidget();
	mUi->setupUi(this);
	mStars.append(mUi->star1);
	mStars.append(mUi->star2);
	mStars.append(mUi->star3);
	mStars.append(mUi->star4);
	mStars.append(mUi->star5);
	mStars.append(mUi->star6);
	mStars.append(mUi->star7);
	mStars.append(mUi->star8);
	mStars.append(mUi->star9);
	mStars.append(mUi->star10);
	for(int i = 0; i < mStars.size(); ++i)
	{
		mStars[i]->setStyleSheet("background: url(" + ViThemeManager::image("star.png", ViThemeImage::Normal, ViThemeManager::Icon).path() + ");");
	}
	setRating(0);
}

ViRatingWidget::~ViRatingWidget()
{
	delete mUi;
}

void ViRatingWidget::setRating(qint16 percentage)
{
	static const qint16 starWidth = 16;
	qint16 fullStars = qFloor(percentage / 10);
	float reduction = (percentage % 10) / 10.0;
	for(int i = 0; i < fullStars; ++i)
	{
		mStars[i]->setFixedWidth(starWidth);
	}
	if(fullStars < mStars.size())
	{
		mStars[fullStars]->setFixedWidth(starWidth * reduction);
		for(int i = fullStars + 1; i < mStars.size(); ++i)
		{
			mStars[i]->setFixedWidth(0);
		}
	}
}
