#include <vierrorfunction.h>
#include <vilogger.h>

ViErrorFunction::ViErrorFunction()
{
	mError = 0;
}

ViErrorFunction::ViErrorFunction(const ViErrorFunction &other)
{
	mError = other.mError;
	mRealValues = other.mRealValues;
	mTargetValues = other.mTargetValues;
}

ViErrorFunction::~ViErrorFunction()
{

}

ViElement ViErrorFunction::exportData()
{
	ViElement element("ErrorFunction");
	element.addChild("Name", name());
	return element;
}

bool ViErrorFunction::importData(ViElement element)
{
	return element.name() == "ErrorFunction" && element.child("Name").toString() == name();
}

void ViErrorFunction::setRealValue(qreal realValue)
{
	setRealValues({realValue});
}

void ViErrorFunction::setRealValues(ViRealList realValues)
{
	ViRealMatrix matrix = {realValues};
	setRealValues(matrix);
}

void ViErrorFunction::setRealValues(ViRealMatrix realValues)
{
	mRealValues = realValues;
}

void ViErrorFunction::setTargetValue(qreal targetValue)
{
	setTargetValues({targetValue});
}

void ViErrorFunction::setTargetValues(ViRealList targetValues)
{
	ViRealMatrix matrix = {targetValues};
	setTargetValues(matrix);
}

void ViErrorFunction::setTargetValues(ViRealMatrix targetValues)
{
	mTargetValues = targetValues;
}

qreal ViErrorFunction::error()
{
	return mError;
}
		
qreal ViErrorFunction::calculate()
{
	bool error = false;
	if(mRealValues.size() == mTargetValues.size())
	{
		for(int i = 0; i < mRealValues.size(); ++i)
		{
			if(mRealValues[i].size() != mTargetValues[i].size())
			{
				error = true;
				break;
			}
		}
	}
	else
	{
		error = true;
	}

	if(error)
	{
		mError = 0;
		LOG("The error could not be calculated, since the real and target value matrices don't have the same size.", QtCriticalMsg);
	}
	else
	{
		mError = calculate(mRealValues, mTargetValues);
	}
	return mError;
}
