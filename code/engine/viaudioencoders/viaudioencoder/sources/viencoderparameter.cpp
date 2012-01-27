#include "viencoderparameter.h"

ViEncoderParameter::ViEncoderParameter(ViEncoderParameter::ViParameterType type, QString name, QString units, QString flag, QString value)
{
	mType = type;
	mName = name;
	mUnits = units;
	mFlag = flag;
	mValue = value;
}

ViEncoderParameter::ViParameterType ViEncoderParameter::type()
{
	return mType;
}

QString ViEncoderParameter::name()
{
	return mName;
}

QString ViEncoderParameter::units()
{
	return mUnits;
}

QString ViEncoderParameter::flag()
{
	return mFlag;
}

QString ViEncoderParameter::value()
{
	return mValue;
}

void ViEncoderParameter::setType(ViEncoderParameter::ViParameterType type)
{
	mType = type;
}

void ViEncoderParameter::setName(QString name)
{
	mName = name;
}

void ViEncoderParameter::setUnits(QString units)
{
	mUnits = units;
}

void ViEncoderParameter::setFlag(QString flag)
{
	mFlag = flag;
}

void ViEncoderParameter::setValue(QString value)
{
	mValue = value;
}

void ViEncoderParameter::setValue(int value)
{
	mValue = QString::number(value);
}

void ViEncoderParameter::setValue(double value)
{
	mValue = QString::number(value, 'f', 8);
}

void ViEncoderParameter::setValue(bool value)
{
	if(value)
	{
		mValue = "true";
	}
	mValue = "false";
}

QMap<QString, QString> ViEncoderParameter::possibleValues()
{
	return mPossibleValues;
}

void ViEncoderParameter::setPossibleValues(QMap<QString, QString> possibleValues)
{
	if(mType == ViEncoderParameter::Enumeration)
	{
		mPossibleValues = possibleValues;
	}
}

void ViEncoderParameter::addPossibleValue(QString name, QString value)
{
	if(mType == ViEncoderParameter::Enumeration)
	{
		mPossibleValues.insert(name, value);
	}
}

QString ViEncoderParameter::string()
{
	if(mType == ViEncoderParameter::Bool && mValue == "true")
	{
		return mFlag.trimmed();
	}
	else if(mType == ViEncoderParameter::Bool && mValue == "false")
	{
		return "";
	}
	QString result = mFlag + " " + mValue;
	return result.trimmed();
}
