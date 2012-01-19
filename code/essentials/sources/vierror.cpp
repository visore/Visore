#include "vierror.h"

ViErrorInfo::ViErrorInfo(QString name, QString description, ViErrorInfo::ViErrorType type)
{
	mName = name;
	mDescription = description;
	mType = type;
}

void ViErrorInfo::setName(QString name)
{
	mName = name;
}

void ViErrorInfo::setDescription(QString description)
{
	mDescription = description;
}

void ViErrorInfo::setType(ViErrorInfo::ViErrorType type)
{
	mType = type;
}

QString ViErrorInfo::name()
{
	return mName;
}

QString ViErrorInfo::description()
{
	return mDescription;
}

ViErrorInfo::ViErrorType ViErrorInfo::type()
{
	return mType;
}

void ViErrorInfo::print()
{
	#ifndef NDEBUG
	QString typeString = "Fatal";
	if(mType == ViErrorInfo::MediumFatal)
	{
		typeString = "Medium Fatal";
	}
	else if(mType == ViErrorInfo::NonFatal)
	{
		typeString = "Non-Fatal";
	}
	ViLogger::debug("*************************************************\nAn error occured:\n\tName: " + mName + "\n\tDescription: " + mDescription + "\n\tType: " + typeString + "\n*************************************************");
	#endif
}

void ViError::setErrorParameters(QString name, QString description, ViErrorInfo::ViErrorType type, bool debugPrint)
{
	ViErrorInfo error(name, description, type);
	if(debugPrint)
	{
		error.print();
	}
	mErrors.append(error);
}

ViErrorInfo ViError::error(int index)
{
	if(index < mErrors.length())
	{
		return mErrors[index];
	}
}

int ViError::numberOfErrors()
{
	return mErrors.length();
}

void ViError::clearErrors()
{
	mErrors.clear();
}
