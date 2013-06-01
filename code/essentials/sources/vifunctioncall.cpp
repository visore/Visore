#include <vifunctioncall.h>
#include <vilogger.h>

#define FUNCTION_CALL_MAX_PARAMETERS 10

ViFunctionCall::ViFunctionCall()
{
	mObject = NULL;
	mFunctionName = "";
}

ViFunctionCall::ViFunctionCall(QObject *object)
{
	mObject = object;
	mFunctionName = "";
}

ViFunctionCall::ViFunctionCall(QString functionName)
{
	mObject = NULL;
	mFunctionName = functionName;
}

ViFunctionCall::ViFunctionCall(QString functionName, bool parameter)
{
	mObject = NULL;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QString functionName, int parameter)
{
	mObject = NULL;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QString functionName, double parameter)
{
	mObject = NULL;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QString functionName, QString parameter)
{
	mObject = NULL;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QString functionName, QVariant parameter)
{
	mObject = NULL;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QString functionName, QList<QVariant> parameters)
{
	mObject = NULL;
	mFunctionName = functionName;
	mParameters = parameters;
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName)
{
	mObject = object;
	mFunctionName = functionName;
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, bool parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, int parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, double parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, QString parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, QVariant parameter)
{
	mObject = object;
	mFunctionName = functionName;
	addParameter(parameter);
}

ViFunctionCall::ViFunctionCall(QObject *object, QString functionName, QList<QVariant> parameters)
{
	mObject = object;
	mFunctionName = functionName;
	mParameters = parameters;
}

ViFunctionCall::ViFunctionCall(const ViFunctionCall &other)
{
	mObject = other.mObject;
	mFunctionName = other.mFunctionName;
	mParameters = other.mParameters;
}

ViFunctionCall::~ViFunctionCall()
{
}

void ViFunctionCall::setObject(QObject *object)
{
	mObject = object;
}

void ViFunctionCall::setFunction(QString functionName)
{
	mFunctionName = functionName;
}

void ViFunctionCall::addParameter(bool parameter)
{
	addParameter(QVariant::fromValue(parameter));
}

void ViFunctionCall::addParameter(int parameter)
{
	addParameter(QVariant::fromValue(parameter));
}

void ViFunctionCall::addParameter(double parameter)
{
	addParameter(QVariant::fromValue(parameter));
}

void ViFunctionCall::addParameter(QString parameter)
{
	addParameter(QVariant::fromValue(parameter));
}

void ViFunctionCall::addParameter(QVariant parameter)
{
	if(mParameters.size() == FUNCTION_CALL_MAX_PARAMETERS)
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
	QList<QGenericArgument> arguments;
	for(int i = 0; i < mParameters.size(); ++i)
	{
		//Q_ARG(mParameters[i].type(), mParameters[i]);
		arguments.append(QGenericArgument(mParameters[i].typeName(), mParameters[i].constData()));
	}
	for(int i = 0; i < FUNCTION_CALL_MAX_PARAMETERS - mParameters.size(); ++i)
	{
		arguments.append(QGenericArgument());
	}

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
