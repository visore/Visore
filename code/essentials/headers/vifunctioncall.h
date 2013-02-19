#ifndef VIFUNCTIONCALL_H
#define VIFUNCTIONCALL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QGenericArgument>
#include <QGenericReturnArgument>
#include <vilogger.h>
#include <QVariant>

typedef QList<QVariant> ViParameterList;

class ViFunctionCall
{

	public:

		ViFunctionCall();
		ViFunctionCall(QObject *object, QString functionName, QVariant parameter);
		ViFunctionCall(QObject *object, QString functionName, bool parameter);
		ViFunctionCall(QObject *object, QString functionName, int parameter);
		ViFunctionCall(QObject *object, QString functionName, float parameter);
		ViFunctionCall(QObject *object, QString functionName, double parameter);
		ViFunctionCall(QObject *object, QString functionName, QString parameter);
		ViFunctionCall(QObject *object, QString functionName, ViParameterList parameters = ViParameterList());
		ViFunctionCall(const ViFunctionCall &other);

		void setObject(QObject *object);
		void setFunction(QString functionName);
		void addParameter(QVariant parameter);

		bool execute(QGenericReturnArgument returnValue = QGenericReturnArgument());

	protected:

		QList<QGenericArgument> createParameters();

	private:

		QObject *mObject;
		QString mFunctionName;
		ViParameterList mParameters;

};

typedef QList<ViFunctionCall> ViFunctionCallList;
typedef QMap<QString, ViFunctionCall> ViFunctionCallSignalMap;

#endif
