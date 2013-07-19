#include "vipositionslider.h"
#include "ui_vipositionslider.h"
#include <QDate>

ViPositionSlider::ViPositionSlider(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViPositionSlider();
	mUi->setupUi(this);
	clear();
	mUi->slider->setTracking(true);
	QObject::connect(mUi->slider, SIGNAL(sliderReleased()), this, SLOT(movePosition()));
	QObject::connect(mUi->slider, SIGNAL(valueChanged(int)), this, SLOT(setPosition(int)));
}

ViPositionSlider::~ViPositionSlider()
{
	delete mUi;
}

void ViPositionSlider::clear()
{
	mUi->positionMinutes->setText("--");
	mUi->positionSeconds->setText("--");
	mUi->durationMinutes->setText("--");
	mUi->durationSeconds->setText("--");
	mUi->slider->blockSignals(true);
	mUi->slider->setValue(0);
	mUi->slider->blockSignals(false);
	mUi->slider->setMaximum(0);
	mHasPosition = false;
	mHasDuration = false;
}

void ViPositionSlider::changePosition(ViAudioPosition position)
{
	QMutexLocker locker(&mMutex);
	mFormat = position.format();
	int seconds = position.position(ViAudioPosition::Seconds);
	if(seconds >= 0 && seconds != mUi->slider->value())
	{
		mHasPosition = true;
		setPosition(seconds);
		if(!mHasDuration || mUi->slider->maximum() < seconds)
		{
			locker.unlock();
			changeDuration(position);
			locker.relock();
		}
		//Block signals to ensure that the signal doesn't cause the output to request a position changed.
		mUi->slider->blockSignals(true);
		mUi->slider->setValue(seconds);
		mUi->slider->blockSignals(false);
		emit positionChanged(position);
	}
}

void ViPositionSlider::changeDuration(ViAudioPosition duration)
{
	QMutexLocker locker(&mMutex);
	int seconds = duration.position(ViAudioPosition::Seconds);
	if(seconds >= 0 && seconds != mUi->slider->maximum())
	{
		mHasDuration = true;
		setDuration(seconds);
		if(!mHasPosition)
		{
			setPosition(0);
		}
		mUi->slider->setMaximum(seconds);
		emit durationChanged(duration);
	}
}

void ViPositionSlider::movePosition()
{
	emit positionMoved(ViAudioPosition(mUi->slider->value(), ViAudioPosition::Seconds, mFormat));
}

QString ViPositionSlider::minutesString(int seconds)
{
	int minutes = seconds / 60;
	QString result = "";
	if(minutes < 10)
	{
		result += "0";
	}
	result += QString::number(minutes);
	return result;
}

QString ViPositionSlider::secondsString(int seconds)
{
	seconds %= 60;
	QString result = "";
	if(seconds < 10)
	{
		result += "0";
	}
	result += QString::number(seconds);
	return result;
}

void ViPositionSlider::setPosition(int seconds)
{
	mUi->positionMinutes->setText(minutesString(seconds));
	mUi->positionSeconds->setText(secondsString(seconds));
}

void ViPositionSlider::setDuration(int seconds)
{
	mUi->durationMinutes->setText(minutesString(seconds));
	mUi->durationSeconds->setText(secondsString(seconds));
}
