#include <virandomweightinitializer.h>
#include <virandomgenerator.h>

#define DEFAULT_LOWER_LIMIT -0.05
#define DEFAULT_UPPER_LIMIT 0.05

ViRandomWeightInitializer::ViRandomWeightInitializer()
	: ViWeightInitializer()
{
	setLimits(DEFAULT_LOWER_LIMIT, DEFAULT_UPPER_LIMIT);
}

ViRandomWeightInitializer::ViRandomWeightInitializer(const ViRandomWeightInitializer &other)
	: ViWeightInitializer(other)
{
	mLowerLimit = other.mLowerLimit;
	mUpperLimit = other.mUpperLimit;
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

ViElement ViRandomWeightInitializer::exportData()
{
	ViElement element = ViWeightInitializer::exportData();
	element.addChild("lowerlimit", lowerLimit());
	element.addChild("upperlimit", upperLimit());
	return element;
}

bool ViRandomWeightInitializer::importData(ViElement element)
{
	if(ViWeightInitializer::importData(element))
	{
		ViElement limit = element.child("lowerlimit");
		if(limit.isNull())
		{
			LOG("Could not retrieve the lower limit.", QtCriticalMsg);
		}
		else
		{
			setLowerLimit(limit.toReal());
		}
		limit = element.child("upperlimit");
		if(limit.isNull())
		{
			LOG("Could not retrieve the upper limit.", QtCriticalMsg);
		}
		else
		{
			setUpperLimit(limit.toReal());
		}
		return true;
	}
	return false;
}

ViWeightInitializer* ViRandomWeightInitializer::clone()
{
	return new ViRandomWeightInitializer(*this);
}

void ViRandomWeightInitializer::initializeWeight(ViSynapse *synapse)
{
	synapse->setWeight(ViRandomGenerator::generate(mLowerLimit, mUpperLimit));
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWeightInitializer* create()
{
	return new ViRandomWeightInitializer();
}

#ifdef __cplusplus
}
#endif
