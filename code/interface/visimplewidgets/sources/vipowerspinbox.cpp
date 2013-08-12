#include <vipowerspinbox.h>
#include <vipowercalculator.h>
#include<vilogger.h>
ViPowerSpinBox::ViPowerSpinBox(QWidget *parent)
	: QSpinBox(parent)
{
	setMinimum(2);
	QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(setPowerValue(int)));
	QObject::connect(this, SIGNAL(powerValueChanged()), this, SLOT(changePowerValue()), Qt::QueuedConnection);
}

ViPowerSpinBox::~ViPowerSpinBox()
{
}

void ViPowerSpinBox::changePowerValue()
{
	emit valueChanged(value());
}

void ViPowerSpinBox::setPowerValue(int value)
{
	if(ViPowerCalculator::isPower(value))
	{
		return;
	}

	int power;
	if(value > mPreviousValue)
	{
		power = ViPowerCalculator::nextPower(value);
		if(power > maximum())
		{
			power = ViPowerCalculator::previousPower(value);
		}
	}
	else
	{
		power = ViPowerCalculator::previousPower(value);
		if(power < minimum())
		{
			power = ViPowerCalculator::nextPower(value);
		}
	}
	mPreviousValue = power;
	if(power != value)
	{
		blockSignals(true);
		setValue(power);
		blockSignals(false);
		emit powerValueChanged();
	}
}

void ViPowerSpinBox::setMinimum(int minimum)
{
	if(minimum < 2)
	{
		minimum = 2;
	}
	QSpinBox::setMinimum(minimum);
	mPreviousValue = minimum;
}

void ViPowerSpinBox::setRange(int minimum, int maximum)
{
	setMinimum(minimum);
	QSpinBox::setMaximum(maximum);
}
