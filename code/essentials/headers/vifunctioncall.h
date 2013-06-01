#ifndef VIFUNCTIONCALL_H
#define VIFUNCTIONCALL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QGenericArgument>
#include <QGenericReturnArgument>
#include <QGenericArgument>

class ViFunctionCall
{

	public:

		ViFunctionCall();
		ViFunctionCall(QObject *object);

		ViFunctionCall(QString functionName);
		ViFunctionCall(QString functionName, bool parameter);
		ViFunctionCall(QString functionName, int parameter);
		ViFunctionCall(QString functionName, double parameter);
		ViFunctionCall(QString functionName, QString parameter);
		ViFunctionCall(QString functionName, QVariant parameter);
		ViFunctionCall(QString functionName, QGenericArgument parameter);
		ViFunctionCall(QString functionName, QList<QGenericArgument> parameters);

		ViFunctionCall(QObject *object, QString functionName);
		ViFunctionCall(QObject *object, QString functionName, bool parameter);
		ViFunctionCall(QObject *object, QString functionName, int parameter);
		ViFunctionCall(QObject *object, QString functionName, double parameter);
		ViFunctionCall(QObject *object, QString functionName, QString parameter);
		ViFunctionCall(QObject *object, QString functionName, QVariant parameter);
		ViFunctionCall(QObject *object, QString functionName, QGenericArgument parameter);
		ViFunctionCall(QObject *object, QString functionName, QList<QGenericArgument> parameters);

		ViFunctionCall(const ViFunctionCall &other);

		virtual ~ViFunctionCall();

		void setObject(QObject *object);
		void setFunction(QString functionName);

		void addParameter(bool parameter);
		void addParameter(int parameter);
		void addParameter(double parameter);
		void addParameter(QString parameter);
		void addParameter(QVariant parameter);
		void addParameter(QGenericArgument parameter);

		bool execute(QGenericReturnArgument returnValue = QGenericReturnArgument());
	
	protected:

		QObject *mObject;
		QString mFunctionName;
		QList<QGenericArgument> mParameters;

};

typedef QList<ViFunctionCall> ViFunctionCallList;
typedef QMap<QString, ViFunctionCall> ViFunctionCallSignalMap;

#endif
