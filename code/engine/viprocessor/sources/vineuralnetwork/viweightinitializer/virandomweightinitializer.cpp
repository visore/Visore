#include <virandomweightinitializer.h>
#include <virandomgenerator.h>

#define DEFAULT_LOWER_LIMIT -0.05
#define DEFAULT_UPPER_LIMIT 0.05

ViRandomWeightInitializer::ViRandomWeightInitializer()
	: ViWeightInitializer()
{
	setLimits(DEFAULT_LOWER_LIMIT, DEFAULT_UPPER_LIMIT);
}

ViRandomWeightInitializer::~ViRandomWeightInitializer()
{
}

void ViRandomWeightInitializer::setLimits(qreal lowerLimit, qreal upperLimit)
{
	setLowerLimit(lowerLimit);
	setUpperLimit(upperLimit);
}

void ViRandomWeightInitializer::setLowerLimit(qreal lowerLimit)
{
	mLowerLimit = lowerLimit;
}

void ViRandomWeightInitializer::setUpperLimit(qreal upperLimit)
{
	mUpperLimit = upperLimit;
}

qreal ViRandomWeightInitializer::lowerLimit()
{
	return mLowerLimit;
}

qreal ViRandomWeightInitializer::upperLimit()
{
	return mUpperLimit;
}

void ViRandomWeightInitializer::initialize(ViSynapse *synapse)
{
	synapse->setWeight(ViRandomGenerator::generate(mLowerLimit, mUpperLimit));
}
