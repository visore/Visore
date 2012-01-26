#ifndef VIENCODERPARAMETER_H
#define VIENCODERPARAMETER_H

#include <QString>
#include <QList>

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
		ViEncoderParameter(ViEncoderParameter::ViParameterType type, QString name, QString flag, QString value = "");
		ViEncoderParameter::ViParameterType type();
		QString name();
		QString flag();
		QString value();
		void setType(ViEncoderParameter::ViParameterType type);
		void setName(QString name);
		void setFlag(QString flag);
		void setValue(QString value);
		void setValue(int value);
		void setValue(double value);
		void setValue(bool value);
		QList<QString> possibleValues();
		void setPossibleValues(QList<QString> possibleValues);
		void addPossibleValue(QString value);
		QString string();

	protected:
		ViEncoderParameter::ViParameterType mType;
		QString mName;
		QString mFlag;
		QString mValue;
		QList<QString> mPossibleValues;
};

#endif
