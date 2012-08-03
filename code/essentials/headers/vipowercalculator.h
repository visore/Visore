#ifndef VIPOWERCALCULATOR_H
#define VIPOWERCALCULATOR_H

class ViPowerCalculator
{
	public:
		
		static int power(int exponent, int power = 2);
		static double exponent(int value, int power = 2);

		static int closestPower(int value, int power = 2);
		static int nextPower(int value, int power = 2);
		static int previousPower(int value, int power = 2);

};

#endif
