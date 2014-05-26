#include "vilogger.h"
#include <QDir>

#define ENABLE_COLOR false

// 0: Lite logging
// 1: Medium logging
// 2: Heavy logging
#define LOGGER_TYPE 1

ViLogEntry::ViLogEntry(QString fileName, QString functionName, int lineNumber, QString message, QtMsgType type)
{
	mFileName = fileName;
	mClassName = "";
	mFunctionName = functionName;
	mMessage = message;
	mLineNumber = lineNumber;
	mType = type;
}

ViLogEntry::ViLogEntry(QString fileName, QString className, QString functionName, int lineNumber, QString message, QtMsgType type)
{
	mFileName = fileName;
	mClassName = className;
	mFunctionName = functionName;
	mMessage = message;
	mLineNumber = lineNumber;
	mType = type;
}

QString ViLogEntry::fileName()
{
	return mFileName;
}

QString ViLogEntry::className()
{
	return mClassName;
}

QString ViLogEntry::functionName()
{
	return mFunctionName;
}

int ViLogEntry::lineNumber()
{
	return mLineNumber;
}

QString ViLogEntry::message()
{
	return mMessage;
}

QtMsgType ViLogEntry::type()
{
	return mType;
}

QString ViLogEntry::toString()
{
	QString result;
	switch(mType)
	{
		case QtDebugMsg:
			result = "Status: ";
			break;
		case QtWarningMsg:
			result = "Warning: ";
			break;
		case QtCriticalMsg:
			result = "Critical Error: ";
			break;
        case QtFatalMsg:
			result = "Fatal Error: ";
	}
	result += mFunctionName + "(" + mClassName + " " + mFileName + ":" + QString::number(mLineNumber) + ") " + mMessage;
	return result;
}

void ViLogEntry::print()
{
    if(LOGGER_TYPE == 0)
	{
		printShort();
	}
	else if(LOGGER_TYPE == 1)
	{
		printMedium();
	}
	else
	{
		printLong();
    }
}

void ViLogEntry::printShort()
{   
	switch(mType)
	{
        case QtDebugMsg:
            cout << greenColor() << "Status: ";
			break;
        case QtWarningMsg:
            cout << yellowColor() << "Warning: ";
			break;
        case QtCriticalMsg:
            cout << redColor() << "Critical Error: ";
			break;
        case QtFatalMsg:
            cout << redColor() << "Fatal Error: ";
    }
    cout << whiteColor() << mMessage. toLatin1().data() << clearColor() << endl;
}

void ViLogEntry::printMedium()
{
    switch(mType)
    {
        case QtDebugMsg:
            cout << greenColor() << "Status: ";
            break;
        case QtWarningMsg:
            cout << yellowColor() << "Warning: ";
            break;
        case QtCriticalMsg:
            cout << redColor() << "Critical Error: ";
            break;
        case QtFatalMsg:
            cout << redColor() << "Fatal Error: ";
    }
    if(mClassName != "")
    {
        cout << blueColor() << mClassName.toLatin1().data() << " ";
	}
    cout << whiteColor() << mMessage. toLatin1().data() << clearColor() << endl;
}

void ViLogEntry::printLong()
{
    switch(mType)
    {
        case QtDebugMsg:
            cout << greenColor() << "Status: ";
            break;
        case QtWarningMsg:
            cout << yellowColor() << "Warning: ";
            break;
        case QtCriticalMsg:
            cout << redColor() << "Critical Error: ";
            break;
        case QtFatalMsg:
            cout << redColor() << "Fatal Error: ";
    }
    if(mClassName != "")
	{
        cout << blueColor() << mClassName.toLatin1().data() << " ";
	}
    cout << whiteColor() << mFileName. toLatin1().data() << ":" << mLineNumber << ") " << whiteColor() << mMessage. toLatin1().data() << clearColor() << endl;
}

string ViLogEntry::clearColor()
{
    if(ENABLE_COLOR) return "\033[0m";
    return "";
}

string ViLogEntry::whiteColor()
{
    if(ENABLE_COLOR) return "\033[1;37m";
    return "";
}

string ViLogEntry::redColor()
{
    if(ENABLE_COLOR) return "\033[1;31m";
    return "";
}

string ViLogEntry::greenColor()
{
    if(ENABLE_COLOR) return "\033[1;32m";
    return "";
}

string ViLogEntry::yellowColor()
{
    if(ENABLE_COLOR) return "\033[1;33m";
    return "";
}

string ViLogEntry::blueColor()
{
    if(ENABLE_COLOR) return "\033[1;34m";
    return "";
}

QSharedPointer<ViLogger> ViLogger::mInstance;

ViLogger::ViLogger()
{
	mTerminalEnabled = true;
}

ViLogger* ViLogger::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViLogger>(new ViLogger);
	}
	return mInstance.data();
}

void ViLogger::enableTerminal(bool enable)
{
	mTerminalEnabled = enable;
}

void ViLogger::append(ViLogEntry entry)
{
	//mEntries.append(entry); // This can cause too much memory consumption
	if(mTerminalEnabled)
	{
		entry.print();
	}
}

void viLog(const char *file, const char *function, const int line, const QString message, QtMsgType type)
{
	QString fileName(file);
	fileName = fileName.mid(fileName.lastIndexOf(QDir::separator()) + 1);
	LOGGER->append(ViLogEntry(fileName, QString(function), line, message, type));
}

void viLog(const char *file, const QString className, const char *function, const int line, const QString message, QtMsgType type)
{
	QString fileName(file);
	fileName = fileName.mid(fileName.lastIndexOf(QDir::separator()) + 1);
	LOGGER->append(ViLogEntry(fileName, className, QString(function), line, message, type));
}
