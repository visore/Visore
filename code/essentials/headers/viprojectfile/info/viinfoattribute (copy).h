#ifndef VIINFOPRIMITIVE_H
#define VIINFOPRIMITIVE_H

#include <QVariant>

class ViInfoPrimitive
{

	public:

		ViInfoPrimitive();
		ViInfoPrimitive(QString name, QString value);
		ViInfoPrimitive(QString name, QVariant value);
		ViInfoPrimitive(QString name, char value);
		ViInfoPrimitive(QString name, int value);
		ViInfoPrimitive(QString name, short value);
		ViInfoPrimitive(QString name, float value);
		ViInfoPrimitive(QString name, double value);
		ViInfoPrimitive(QString name, qint64 value);
		ViInfoPrimitive(QString name, bool value);
		ViInfoPrimitive(const ViInfoPrimitive &other);

		QString name();
		QVariant value();
		QString valueString();
		int valueInt();
		double valueDouble();
		bool valueBool();

		void setName(QString name);
		void setValue(QString value);
		void setValue(QVariant value);
		void setValue(char value);
		void setValue(int value);
		void setValue(short value);
		void setValue(float value);
		void setValue(double value);
		void setValue(qint64 value);
		void setValue(bool value);

	private:

		QString mName;
		QVariant mValue;

};

#endif
