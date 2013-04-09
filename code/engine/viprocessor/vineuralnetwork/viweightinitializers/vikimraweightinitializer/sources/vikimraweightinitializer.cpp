#include <vikimraweightinitializer.h>
#include <virandomgenerator.h>
#include <QtCore/qmath.h>

#define DEFAULT_UPPER_LIMIT 0.1

ViKimRaWeightInitializer::ViKimRaWeightInitializer()
	: ViWeightInitializer()
{
	setUpperLimit(DEFAULT_UPPER_LIMIT);
}

ViKimRaWeightInitializer::ViKimRaWeightInitializer(const ViKimRaWeightInitializer &other)
	: ViWeightInitializer(other)
{
	mUpperLimit = other.mUpperLimit;
}

ViKimRaWeightInitializer::~ViKimRaWeightInitializer()
{
}

void ViKimRaWeightInitializer::setUpperLimit(qreal upperLimit)
{
	mUpperLimit = upperLimit;
}

qreal ViKimRaWeightInitializer::upperLimit()
{
	return mUpperLimit;
}

ViElement ViKimRaWeightInitializer::exportData()
{
	ViElement element = ViWeightInitializer::exportData();
	element.addChild("UpperLimit", upperLimit());
	return element;
}

bool ViKimRaWeightInitializer::importData(ViElement element)
{
	if(ViWeightInitializer::importData(element))
	{
		ViElement limit = element.child("UpperLimit");
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

ViWeightInitializer* ViKimRaWeightInitializer::clone()
{
	return new ViKimRaWeightInitializer(*this);
}

void ViKimRaWeightInitializer::initializeWeight(ViSynapse *synapse)
{
	qreal lowerLimit = qSqrt(currentLearningRate() / currentNeuronCount());
	synapse->setWeight(ViRandomGenerator::generate(lowerLimit, mUpperLimit));
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWeightInitializer* create()
{
	return new ViKimRaWeightInitializer();
}

#ifdef __cplusplus
}
#endif
