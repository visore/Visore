#include "vivalue.h"

ViValue::ViValue()
{
	setName("");
}

ViValue::ViValue(const QString name)
{
	setName(name);
}

ViValue::ViValue(const QString name, const QVariant value)
{
	setName(name);
	setValue(value);
}

ViValue::ViValue(const ViValue &other)
{
	mName = other.mName;
	mValue = other.mValue;
}

QString ViValue::name() const
{
	return mName;
}

QVariant ViValue::value() const
{
	return mValue;
}

void ViValue::setName(const QString name)
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

void ViValue::setValue(const QVariant value)
{
	mValue = value;
}

QString ViValue::toString() const
{
	return mValue.toString();
}

bool ViValue::toBool() const
{
	return mValue.toBool();
}

qint ViValue::toInt() const
{
	return mValue.toInt();
}

qreal ViValue::toReal() const
{
	return mValue.toReal();
}

QBoolList ViValue::toBoolList() const
{
	QList<bool> result;
	QList<QVariant> list = mValue.toList();
	for(int i = 0; i < list.size(); ++i)
	{
		result.append(list[i].toBool());
	}
	return result;
}

QStringList ViValue::toStringList() const
{
	return mValue.toStringList();
}

QIntList ViValue::toIntList() const
{
	QList<qint> result;
	QList<QVariant> list = mValue.toList();
	for(int i = 0; i < list.size(); ++i)
	{
		result.append(list[i].toInt());
	}
	return result;
}


QRealList ViValue::toRealList() const
{
	QList<qreal> result;
	QList<QVariant> list = mValue.toList();
	for(int i = 0; i < list.size(); ++i)
	{
		result.append(list[i].toReal());
	}
	return result;
}

bool ViValue::isNull() const
{
	return mIsNull;
}
