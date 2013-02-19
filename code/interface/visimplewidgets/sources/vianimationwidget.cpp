#include "vianimationwidget.h"

ViAnimationWidget::ViAnimationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mLayout = new QGridLayout(this);
	mLayout->setParent(this);
	mLabel = new QLabel("");
	mLayout->addWidget(mLabel);
	mLayout->setContentsMargins(0, 0, 0, 0);

	mCurrentPixmap = 0;

	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(next()));
}

ViAnimationWidget::~ViAnimationWidget()
{
	delete mLayout;
}

void ViAnimationWidget::start(int interval)
{
	next();
	mTimer.start(interval);
}

void ViAnimationWidget::stop()
{
	mTimer.stop();
}

void ViAnimationWidget::next()
{
	if(mCurrentPixmap < mPixmaps.size())
	{
		mLabel->setPixmap(mPixmaps[mCurrentPixmap]);
		++mCurrentPixmap;
		if(mCurrentPixmap >= mPixmaps.size())
		{
			mCurrentPixmap = 0;
		}
	}
	else
	{
		stop();
	}
}

void ViAnimationWidget::setAnimation(ViThemeAnimation animation, int size)
{
	mPixmaps = animation.pixmaps(size);
}
