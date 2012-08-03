#include "vipowercalculator.h"
#include "math.h"
#include <qmath.h>

int ViPowerCalculator::power(int exponent, int power)
{
	return pow(power, exponent);
}

double ViPowerCalculator::exponent(int value, int power)
{
	return log(value) / log(power);
}

int ViPowerCalculator::closestPower(int value, int power)
{
	int next = ViPowerCalculator::nextPower(value, power);
	int previous = ViPowerCalculator::previousPower(value, power);
	if(qAbs(value - previous) < qAbs(value - next))
	{
		return previous;
	}
	return next;
}

int ViPowerCalculator::nextPower(int value, int power)
{
	return ViPowerCalculator::power(qCeil(ViPowerCalculator::exponent(value, power)), power);
}

int ViPowerCalculator::previousPower(int value, int power)
{
	return ViPowerCalculator::power(qFloor(ViPowerCalculator::exponent(value, power)), power);
}
