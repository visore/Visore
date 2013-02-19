#ifndef VISIZECONVERTER_H
#define VISIZECONVERTER_H

#include <vicommon.h>
#include <QString>

class ViSizeConverter
{
	public:
		
		// Assigned values are important
		enum Unit
		{
			Bytes = 0,
			KiloBytes = 1,
			MegaBytes = 2,
			GigaBytes = 3,
			TeraBytes = 4
		};

		static qreal convert(qreal value, ViSizeConverter::Unit inputUnit, ViSizeConverter::Unit outputUnit);
		static qreal convertToBytes(qreal value, ViSizeConverter::Unit unit);
		static qreal convertToKiloBytes(qreal value, ViSizeConverter::Unit unit);
		static qreal convertToMegaBytes(qreal value, ViSizeConverter::Unit unit);
		static qreal convertToGigaBytes(qreal value, ViSizeConverter::Unit unit);
		static qreal convertToTeraBytes(qreal value, ViSizeConverter::Unit unit);

		static QString toOptimalString(qreal value, ViSizeConverter::Unit unit, short decimalPlaces = 2);
		static QString toString(qreal value, ViSizeConverter::Unit unit, short decimalPlaces = 2);
		static QString toString(qreal value, ViSizeConverter::Unit inputUnit, ViSizeConverter::Unit outputUnit, short decimalPlaces = 2);

	private:

		static QString unitString(ViSizeConverter::Unit unit);
		static ViSizeConverter::Unit optimalUnit(qreal value, ViSizeConverter::Unit unit);

};

#endif
