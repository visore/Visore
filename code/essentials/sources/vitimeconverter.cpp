#include <vitimeconverter.h>
#include <QTime>

qreal ViTimeConverter::convert(qreal value, ViTimeConverter::Unit inputUnit, ViTimeConverter::Unit outputUnit)
{
	int milliseconds;

	if(inputUnit == ViTimeConverter::Days)
	{
		milliseconds = value * 86400000;
	}
	else if(inputUnit == ViTimeConverter::Hours)
	{
		milliseconds = value * 3600000;
	}
	else if(inputUnit == ViTimeConverter::Minutes)
	{
		milliseconds = value * 60000;
	}
	else if(inputUnit == ViTimeConverter::Seconds)
	{
		milliseconds = value * 1000;
	}
	else
	{
		milliseconds = value;
	}

	if(outputUnit == ViTimeConverter::Days)
	{
		value = milliseconds / 86400000;
	}
	else if(outputUnit == ViTimeConverter::Hours)
	{
		value = milliseconds / 3600000;
	}
	else if(outputUnit == ViTimeConverter::Minutes)
	{
		value = milliseconds / 60000;
	}
	else if(outputUnit == ViTimeConverter::Seconds)
	{
		value = milliseconds / 1000;
	}

	return value;
}

qreal ViTimeConverter::convertToMilliseconds(qreal value, ViTimeConverter::Unit unit)
{
	return convert(value, unit, ViTimeConverter::Milliseconds);
}

qreal ViTimeConverter::convertToSeconds(qreal value, ViTimeConverter::Unit unit)
{
	return convert(value, unit, ViTimeConverter::Seconds);
}

qreal ViTimeConverter::convertToMinutes(qreal value, ViTimeConverter::Unit unit)
{
	return convert(value, unit, ViTimeConverter::Minutes);
}

qreal ViTimeConverter::convertToHours(qreal value, ViTimeConverter::Unit unit)
{
	return convert(value, unit, ViTimeConverter::Hours);
}

qreal ViTimeConverter::convertToDays(qreal value, ViTimeConverter::Unit unit)
{
	return convert(value, unit, ViTimeConverter::Days);
}

QString ViTimeConverter::toOptimalString(qreal value, ViTimeConverter::Unit unit)
{
	QTime time(0, 0, 0, 0);
	time = time.addMSecs(convert(value, unit, ViTimeConverter::Milliseconds));
	return time.toString("hh:mm:ss");
}

QString ViTimeConverter::toString(qreal value, ViTimeConverter::Unit unit, short decimalPlaces)
{
	return QString::number(value, 'f', decimalPlaces) + " " + unitString(unit);
}

QString ViTimeConverter::toString(qreal value, ViTimeConverter::Unit inputUnit, ViTimeConverter::Unit outputUnit, short decimalPlaces)
{
	return toString(convert(value, inputUnit, outputUnit), outputUnit, decimalPlaces);
}

QString ViTimeConverter::unitString(ViTimeConverter::Unit unit)
{
	if(unit == ViTimeConverter::Milliseconds)
	{
		return "msecs";
	}
	else if(unit == ViTimeConverter::Seconds)
	{
		return "secs";
	}
	else if(unit == ViTimeConverter::Minutes)
	{
		return "mins";
	}
	else if(unit == ViTimeConverter::Hours)
	{
		return "hours";
	}
	else if(unit == ViTimeConverter::Days)
	{
		return "days";
	}
}
