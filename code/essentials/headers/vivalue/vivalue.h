#ifndef VIVALUE_H
#define VIVALUE_H

#include <QVariant>
#include <QList>
#include <vicommon.h>

class ViValue
{

	public:

		ViValue();
		ViValue(const QString name);
		ViValue(const QString name, const QVariant value);
		ViValue(const ViValue &other);
		virtual ~ViValue();

		QString name() const;
		QVariant value() const;

		void setName(const QString name);
		void setValue(const QVariant value);
		
		QString toString() const;
		bool toBool() const;
		qint toInt() const;
		qreal toReal() const;
		ViBoolList toBoolList() const;
		ViStringList toStringList() const;
		ViIntegerList toIntList() const;
		ViRealList toRealList() const;

		bool isNull() const;

	private:

		bool mIsNull;
		QString mName;
		QVariant mValue;

};

typedef QList<ViValue> ViValueList;

#endif
