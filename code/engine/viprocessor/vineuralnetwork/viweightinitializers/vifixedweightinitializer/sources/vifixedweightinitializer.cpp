#include <vifixedweightinitializer.h>

ViFixedWeightInitializer::ViFixedWeightInitializer()
	: ViWeightInitializer()
{
	clear();
}

ViFixedWeightInitializer::ViFixedWeightInitializer(const ViFixedWeightInitializer &other)
	: ViWeightInitializer(other)
{
	mValues = other.mValues;
	mCurrentValue = other.mCurrentValue;
	mDefaultValue = other.mDefaultValue;
}

ViFixedWeightInitializer::~ViFixedWeightInitializer()
{
}

void ViFixedWeightInitializer::setValues(qreal value)
{
	clear();
	mDefaultValue = value;
}

void ViFixedWeightInitializer::setValues(ViRealList values)
{
	clear();
	mValues = values;
}

void ViFixedWeightInitializer::setValues(ViRealMatrix values)
{
	clear();
	for(int i = 0; i < values.size(); ++i)
	{
		for(int j = 0; j < values[i].size(); ++j)
		{
			mValues.append(values[i][j]);
		}
	}
}

void ViFixedWeightInitializer::clear()
{
	mValues.clear();
	mCurrentValue = -1;
	mDefaultValue = 1;
}

ViElement ViFixedWeightInitializer::exportData()
{
	ViElement element = ViWeightInitializer::exportData();
	element.addChild("DefaultValue", mDefaultValue);
	if(mValues.size() > 0)
	{
		ViElement values("Values");
		for(int i = 0; i < mValues.size(); ++i)
		{
			values.addChild("Value", mValues[i]);
		}
		element.addChild(values);
	}
	return element;
}

bool ViFixedWeightInitializer::importData(ViElement element)
{
	if(ViWeightInitializer::importData(element))
	{
		clear();
		ViElement value = element.child("DefaultValue");
		if(value.isNull())
		{
			LOG("Could not retrieve the default value.", QtCriticalMsg);
		}
		else
		{
			setValues(value.toReal());
		}
		value = element.child("Values");
		if(!value.isNull())
		{
			ViElementList values = value.children("Values");
			ViRealList realValues;
			for(int i = 0; i < values.size(); ++i)
			{
				realValues.append(values[i].toReal());
			}
			setValues(realValues);
		}
		return true;
	}
	return false;
}

ViWeightInitializer* ViFixedWeightInitializer::clone()
{
	return new ViFixedWeightInitializer(*this);
}

void ViFixedWeightInitializer::initializeWeight(ViSynapse *synapse)
{
	++mCurrentValue;
	if(mCurrentValue < mValues.size())
	{
		synapse->setWeight(mValues[mCurrentValue]);
	}
	else
	{
		synapse->setWeight(mDefaultValue);
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWeightInitializer* create()
{
	return new ViFixedWeightInitializer();
}

#ifdef __cplusplus
}
#endif
