#include <vifunctioncall.h>
#include <vilogger.h>

#define MAXIMUM_PARAMETERS 10

ViFunctionCall::ViFunctionCall()
{
	mObject = NULL;
	mFunctionName = "";
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, QVariant parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, bool parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(QVariant(parameter));
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, int parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(QVariant(parameter));
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, float parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(QVariant(parameter));
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, double parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(QVariant(parameter));
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, QString parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(QVariant(parameter));
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, ViParameterList parameters)
{
	mObject = object;
	mFunctionName = functionName;
	for(int i = 0; i < parameters.size(); ++i)
	{
		addParameter(parameters[i]);
	}
}

ViFunctionCall::ViFunctionCall(const ViFunctionCall &other)
{
	mObject = other.mObject;
	mFunctionName = other.mFunctionName;
	mParameters = other.mParameters;
}

void ViFunctionCall::setObject(QObject *object)
{
	mObject = object;
}

void ViFunctionCall::setFunction(QString functionName)
{
	mFunctionName = functionName;
}

void ViFunctionCall::addParameter(QVariant parameter)
{
	if(mParameters.size() == MAXIMUM_PARAMETERS)
	{
		LOG("Cannot add parameter. A maximum of 10 parameters is allowed.");
	}
	else
	{
		mParameters.append(parameter);
	}
}

bool ViFunctionCall::execute(QGenericReturnArgument returnValue)
{
	if(mObject == NULL)
	{
		return false;
	}
	QList<QGenericArgument> arguments = createParameters();
	return QMetaObject::invokeMethod(mObject, mFunctionName.toLatin1().data(), Qt::DirectConnection,
		arguments[0],
		arguments[1],
		arguments[2],
		arguments[3],
		arguments[4],
		arguments[5],
		arguments[6],
		arguments[7],
		arguments[8],
		arguments[9]
	);
}

QList<QGenericArgument> ViFunctionCall::createParameters()
{
	QList<QGenericArgument> result;
	for(int i = 0; i < mParameters.size(); ++i)
	{
		result.append(Q_ARG(QVariant, mParameters[i]));
	}
	for(int i = 0; i < MAXIMUM_PARAMETERS - mParameters.size(); ++i)
	{
		result.append(QGenericArgument());
	}
	return result;
}
