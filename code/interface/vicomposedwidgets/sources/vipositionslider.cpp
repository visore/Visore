#include "vipositionslider.h"
#include "ui_vipositionslider.h"
#include <QDate>

ViPositionSlider::ViPositionSlider(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViPositionSlider();
	mUi->setupUi(this);
	clear();
	QObject::connect(engine().data(), SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(changedPosition(ViAudioPosition)));
	QObject::connect(engine().data(), SIGNAL(lengthChanged(ViAudioPosition)), this, SLOT(changedLength(ViAudioPosition)));
	QObject::connect(mUi->slider, SIGNAL(valueChanged(int)), engine().data(), SLOT(setPosition(int)));
}

ViPositionSlider::~ViPositionSlider()
{
	delete mUi;
}

void ViPositionSlider::clear()
{
	mUi->positionMinutes->setText("--");
	mUi->positionSeconds->setText("--");
	mUi->lengthMinutes->setText("--");
	mUi->lengthSeconds->setText("--");
	mUi->slider->blockSignals(true);
	mUi->slider->setValue(0);
	mUi->slider->blockSignals(false);
	mUi->slider->setMaximum(0);
	mHasPosition = false;
	mHasLength = false;
}

void ViPositionSlider::changedPosition(ViAudioPosition position)
{
	int seconds = position.position(ViAudioPosition::Seconds);
	if(seconds > 0)
	{
		mHasPosition = true;
		setPosition(seconds);
		if(!mHasLength)
		{
			changedLength(position);
		}
		//Block signals to ensure that the signal doesn't cause the output to request a position changed.
		mUi->slider->blockSignals(true);
		mUi->slider->setValue(seconds);
		mUi->slider->blockSignals(false);
	}
}

void ViPositionSlider::changedLength(ViAudioPosition length)
{
	int seconds = length.position(ViAudioPosition::Seconds);
	if(seconds > 0)
	{
		mHasLength = true;
		setLength(seconds);
		if(!mHasPosition)
		{
			setPosition(0);
		}
		mUi->slider->setMaximum(seconds);
	}
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

void ViPositionSlider::setLength(int seconds)
{
	mUi->lengthMinutes->setText(minutesString(seconds));
	mUi->lengthSeconds->setText(secondsString(seconds));
}
