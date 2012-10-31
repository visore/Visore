#ifndef VIINFOPRIMITIVE_H
#define VIINFOPRIMITIVE_H

#include <QVariant>
#include <QList>

class ViInfoPrimitive
{

	public:

		ViInfoPrimitive();
		ViInfoPrimitive(QString name);
		ViInfoPrimitive(QString name, QVariant value);
		ViInfoPrimitive(const ViInfoPrimitive &other);

		QString name();
		QVariant value();
		QString valueString();
		int valueInt();
		double valueDouble();
		bool valueBool();

		void setName(QString name);
		void setValue(QVariant value);

		bool isNull();

	private:

		bool mIsNull;
		QString mName;
		QVariant mValue;

};

typedef QList<ViInfoPrimitive> ViInfoPrimitiveList;

#endif
