#include <vierrorfunction.h>
#include <vilogger.h>

ViErrorFunction::ViErrorFunction()
{
	clear();
}

ViErrorFunction::ViErrorFunction(const ViErrorFunction &other)
{
	mError = other.mError;
	mCount = other.mCount;
}

ViErrorFunction::~ViErrorFunction()
{
}

ViElement ViErrorFunction::exportData()
{
	ViElement element(name(), error());
	return element;
}

bool ViErrorFunction::importData(ViElement element)
{
	return element.name() == name();
}

qreal ViErrorFunction::add(const qreal &realValue, const qreal &targetValue)
{
	++mCount;
	mError = calculate(realValue, targetValue);
	return mError;
}

qreal ViErrorFunction::add(const ViDoubleList &realValues, const ViDoubleList &targetValues)
{
	++mCount;
	if(realValues.size() != targetValues.size())
	{
		LOG("The amount of real and target values doesn't correspond. The values will be adapted.");
		int difference = realValues.size() - targetValues.size();
		if(difference < 0)
		{
			difference = qAbs(difference);
			ViDoubleList newList = targetValues;
			for(int i = 0; i < difference; ++i)
			{
				newList.removeLast();
			}
			mError = calculate(realValues, newList);
		}
		else
		{
			ViDoubleList newList = realValues;
			for(int i = 0; i < difference; ++i)
			{
				newList.removeLast();
			}
			mError = calculate(newList, targetValues);
		}
	}
	else
	{
		mError = calculate(realValues, targetValues);
	}
	return mError;
}

void ViErrorFunction::clear()
{
	mError = 0;
	mCount = 0;
	clearValues();
}

qreal ViErrorFunction::error()
{
	return mError;
}

int ViErrorFunction::count()
{
	return mCount;
}

QString ViErrorFunction::name()
{
	return ViSerializer::name().replace("Function", "");
}
