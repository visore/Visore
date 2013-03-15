#include <virandomgenerator.h>

qreal ViRandomGenerator::generate(qreal lowerLimit, qreal upperLimit)
{
	static bool initialized = initialize();
	return ((double(rand()) / RAND_MAX) * (upperLimit - lowerLimit)) + lowerLimit;
}

qreal ViRandomGenerator::generate(int lowerLimit, qreal upperLimit)
{
	return generate(qreal(lowerLimit), upperLimit);
}

qreal ViRandomGenerator::generate(qreal lowerLimit, int upperLimit)
{
	return generate(lowerLimit, qreal(upperLimit));
}

int ViRandomGenerator::generate(int lowerLimit, int upperLimit)
{
	static bool initialized = initialize();
	return int((double(rand()) / RAND_MAX) * (upperLimit - lowerLimit + 1)) + lowerLimit;
}

bool ViRandomGenerator::initialize()
{
	srand(time(NULL));
}
