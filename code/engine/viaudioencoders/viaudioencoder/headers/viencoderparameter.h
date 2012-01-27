#ifndef VIENCODERPARAMETER_H
#define VIENCODERPARAMETER_H

#include <QString>
#include <QMap>

class ViEncoderParameter
{
	public:
		enum ViParameterType
		{
			Bool = 0,
			Integer = 1,
			Double = 2,
			String = 3,
			Enumeration = 4
		};
		ViEncoderParameter(ViEncoderParameter::ViParameterType type, QString name, QString units, QString flag, QString value = "");
		ViEncoderParameter::ViParameterType type();
		QString name();
		QString units();
		QString flag();
		QString value();
		void setType(ViEncoderParameter::ViParameterType type);
		void setName(QString name);
		void setUnits(QString units);
		void setFlag(QString flag);
		void setValue(QString value);
		void setValue(int value);
		void setValue(double value);
		void setValue(bool value);
		QMap<QString, QString> possibleValues();
		void setPossibleValues(QMap<QString, QString> possibleValues);
		void addPossibleValue(QString name, QString value);
		QString string();

	protected:
		ViEncoderParameter::ViParameterType mType;
		QString mName;
		QString mUnits;
		QString mFlag;
		QString mValue;
		QMap<QString, QString> mPossibleValues;
};

#endif
