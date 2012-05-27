#include "vipowercalculator.h"
#include "math.h"

int ViPowerCalculator::power(int exponent, int power)
{
	return pow(power, exponent);
}

double ViPowerCalculator::exponent(int value, int power)
{
	return log(value) / log(power);
}

int ViPowerCalculator::nextPower(int value, int power)
{
	return ViPowerCalculator::power(power, ceil(ViPowerCalculator::exponent(value, power)));
}

int ViPowerCalculator::previousPower(int value, int power)
{
	return ViPowerCalculator::power(power, floor(ViPowerCalculator::exponent(value, power)));
}
