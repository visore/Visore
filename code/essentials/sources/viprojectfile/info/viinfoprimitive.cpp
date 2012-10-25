#include "viinfoprimitive.h"
#include "vilogger.h"
ViInfoPrimitive::ViInfoPrimitive()
{
	setName("");
}

ViInfoPrimitive::ViInfoPrimitive(QString name)
{
	setName(name);
}

ViInfoPrimitive::ViInfoPrimitive(QString name, QVariant value)
{
	setName(name);
	setValue(value);
}

ViInfoPrimitive::ViInfoPrimitive(const ViInfoPrimitive &other)
{
	mName = other.mName;
	mValue = other.mValue;
}

QString ViInfoPrimitive::name()
{
	return mName;
}

QVariant ViInfoPrimitive::value()
{
	return mValue;
}

QString ViInfoPrimitive::valueString()
{
	return mValue.toString();
}

int ViInfoPrimitive::valueInt()
{
	return mValue.toInt();
}

double ViInfoPrimitive::valueDouble()
{
	return mValue.toDouble();
}

bool ViInfoPrimitive::valueBool()
{
	return mValue.toBool();
}

void ViInfoPrimitive::setName(QString name)
{
	mName = name;
	if(mName == "")
	{
		mIsNull = true;
	}
	else
	{
		mIsNull = false;
	}
}

void ViInfoPrimitive::setValue(QVariant value)
{
	mValue = value;
}

bool ViInfoPrimitive::isNull()
{
	return mIsNull;
}
