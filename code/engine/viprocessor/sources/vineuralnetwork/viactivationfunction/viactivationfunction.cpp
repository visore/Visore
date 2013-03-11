#include <viactivationfunction.h>
#include <viscaler.h>
#include <vilogger.h>

ViActivationFunction::ViActivationFunction(QString name, double functionMinimum, double functionMaximum)
{
	mName = name;
	if(mName.startsWith("Vi"))
	{
		mName.remove(0, 2);
	}

	mFunctionMinimum = functionMinimum;
	mFunctionMaximum = functionMaximum;
	mMinimum = -1;
	mMaximum = 1;
}

ViActivationFunction::ViActivationFunction(const ViActivationFunction &other)
{
	mFunctionMinimum = other.mFunctionMinimum;
	mFunctionMaximum = other.mFunctionMaximum;
	mMinimum = other.mMinimum;
	mMaximum = other.mMaximum;
	mName = other.mName;
}

ViActivationFunction::~ViActivationFunction()
{
}

void ViActivationFunction::setName(QString name)
{
	mName = name;
}

QString ViActivationFunction::name()
{
	return mName;
}

void ViActivationFunction::setRange(double minimum, double maximum)
{
	setMinimum(minimum);
	setMaximum(maximum);
}

void ViActivationFunction::setMinimum(double minimum)
{
	mMinimum = minimum;
}

void ViActivationFunction::setMaximum(double maximum)
{
	mMaximum = maximum;
}

double ViActivationFunction::minimum() const
{
	return mMinimum;
}

double ViActivationFunction::maximum() const
{
	return mMaximum;
}

void ViActivationFunction::setFunctionRange(double minimum, double maximum)
{
	setFunctionMinimum(minimum);
	setFunctionMaximum(maximum);
}

void ViActivationFunction::setFunctionMinimum(double minimum)
{
	mFunctionMinimum = minimum;
}

void ViActivationFunction::setFunctionMaximum(double maximum)
{
	mFunctionMaximum = maximum;
}

double ViActivationFunction::calculate(const double &input, const int &inputCount)
{
	return ViScaler::scale(execute(input, inputCount), mFunctionMinimum, mFunctionMaximum, mMinimum, mMaximum);
}

ViActivationFunction* ViActivationFunction::create(QString name)
{
	int type = QMetaType::type(name.toLatin1().data());
	if(type == QMetaType::UnknownType)
	{
		return NULL;
	}
	return (ViActivationFunction*) QMetaType::create(type);
}

ViElement ViActivationFunction::exportData()
{
	ViElement element("ActivationFunction");
	element.addChild("Name", name());
	element.addChild("Minimum", minimum());
	element.addChild("Maximum", maximum());
	return element;
}

bool ViActivationFunction::importData(ViElement element)
{
	if(element.name() != "ActivationFunction")
	{
		return false;
	}

	ViElement theName = element.child("name");
	if(theName.isNull())
	{
		return false;
	}
	if(theName.toString() != name())
	{
		return false;
	}

	ViElement minimum = element.child("Minimum");
	if(minimum.isNull())
	{
		LOG("The minimum could not be imported", QtCriticalMsg);
	}
	else
	{
		setMinimum(minimum.toReal());
	}
	ViElement maximum = element.child("Maximum");
	if(maximum.isNull())
	{
		LOG("The maximum could not be imported", QtCriticalMsg);
	}
	else
	{
		setMaximum(maximum.toReal());
	}
	return true;
}
