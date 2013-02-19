#include <visizeconverter.h>

#define UNITSIZE 1024

qreal ViSizeConverter::convert(qreal value, ViSizeConverter::Unit inputUnit, ViSizeConverter::Unit outputUnit)
{
	short times = inputUnit - outputUnit;
	if(times < 0)
	{
		times = qAbs(times);
		for(int i = 0; i < times; ++i)
		{
			value /= UNITSIZE;
		}
	}
	else
	{
		for(int i = 0; i < times; ++i)
		{
			value *= UNITSIZE;
		}
	}
	return value;
}

qreal ViSizeConverter::convertToBytes(qreal value, ViSizeConverter::Unit unit)
{
	return convert(value, unit, ViSizeConverter::Bytes);
}

qreal ViSizeConverter::convertToKiloBytes(qreal value, ViSizeConverter::Unit unit)
{
	return convert(value, unit, ViSizeConverter::KiloBytes);
}

qreal ViSizeConverter::convertToMegaBytes(qreal value, ViSizeConverter::Unit unit)
{
	return convert(value, unit, ViSizeConverter::MegaBytes);
}

qreal ViSizeConverter::convertToGigaBytes(qreal value, ViSizeConverter::Unit unit)
{
	return convert(value, unit, ViSizeConverter::GigaBytes);
}

qreal ViSizeConverter::convertToTeraBytes(qreal value, ViSizeConverter::Unit unit)
{
	return convert(value, unit, ViSizeConverter::TeraBytes);
}

QString ViSizeConverter::toOptimalString(qreal value, ViSizeConverter::Unit unit, short decimalPlaces)
{
	return toString(value, unit, optimalUnit(value, unit), decimalPlaces);
}

QString ViSizeConverter::toString(qreal value, ViSizeConverter::Unit unit, short decimalPlaces)
{
	return QString::number(value, 'f', decimalPlaces) + " " + unitString(unit);
}

QString ViSizeConverter::toString(qreal value, ViSizeConverter::Unit inputUnit, ViSizeConverter::Unit outputUnit, short decimalPlaces)
{
	return toString(convert(value, inputUnit, outputUnit), outputUnit, decimalPlaces);
}

QString ViSizeConverter::unitString(ViSizeConverter::Unit unit)
{
	if(unit == ViSizeConverter::Bytes)
	{
		return "B";
	}
	else if(unit == ViSizeConverter::KiloBytes)
	{
		return "KB";
	}
	else if(unit == ViSizeConverter::MegaBytes)
	{
		return "MB";
	}
	else if(unit == ViSizeConverter::GigaBytes)
	{
		return "GB";
	}
	else if(unit == ViSizeConverter::TeraBytes)
	{
		return "TB";
	}
}

ViSizeConverter::Unit ViSizeConverter::optimalUnit(qreal value, ViSizeConverter::Unit unit)
{
	short result = unit;
	while(value >= UNITSIZE)
	{
		value /= UNITSIZE;
		++result;
	}
	while(value < 1)
	{
		value *= UNITSIZE;
		--result;
	}
	return (ViSizeConverter::Unit) result;
}
