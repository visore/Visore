#ifndef VITIMECONVERTER_H
#define VITIMECONVERTER_H

#include <vicommon.h>
#include <QString>

class ViTimeConverter
{
	public:
		
		// Assigned values are important
		enum Unit
		{
			Milliseconds = 0,
			Seconds = 1,
			Minutes = 2,
			Hours = 3,
			Days = 4
		};

		static qreal convert(qreal value, ViTimeConverter::Unit inputUnit, ViTimeConverter::Unit outputUnit);
		static qreal convertToMilliseconds(qreal value, ViTimeConverter::Unit unit);
		static qreal convertToSeconds(qreal value, ViTimeConverter::Unit unit);
		static qreal convertToMinutes(qreal value, ViTimeConverter::Unit unit);
		static qreal convertToHours(qreal value, ViTimeConverter::Unit unit);
		static qreal convertToDays(qreal value, ViTimeConverter::Unit unit);

		static QString toOptimalString(qreal value, ViTimeConverter::Unit unit);
		static QString toString(qreal value, ViTimeConverter::Unit unit, short decimalPlaces = 2);
		static QString toString(qreal value, ViTimeConverter::Unit inputUnit, ViTimeConverter::Unit outputUnit, short decimalPlaces = 2);

	private:

		static QString unitString(ViTimeConverter::Unit unit);

};

#endif
