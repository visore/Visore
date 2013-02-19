#include "vilogger.h"
#include <QDir>

// 0: Lite logging
// 1: Medium logging
// 2: Heavy logging
#define LOGGER_TYPE 1

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
			cout << "\033[1;32mStatus: ";
			break;
		case QtWarningMsg:
			cout << "\033[1;33mWarning: ";
			break;
		case QtCriticalMsg:
			cout << "\033[1;31mCritical Error: ";
			break;
        case QtFatalMsg:
			cout << "\033[1;31mFatal Error: ";
	}
	cout << "\033[1;37m" << mMessage. toLatin1().data() << " \033[0m" << endl;
}

void ViLogEntry::printMedium()
{
	switch(mType)
	{
		case QtDebugMsg:
			cout << "\033[1;32mStatus: ";
			break;
		case QtWarningMsg:
			cout << "\033[1;33mWarning: ";
			break;
		case QtCriticalMsg:
			cout << "\033[1;31mCritical Error: ";
			break;
        case QtFatalMsg:
			cout << "\033[1;31mFatal Error: ";
	}
	cout << "\033[1;34m" << mClassName. toLatin1().data() << "\033[1;37m " << mMessage. toLatin1().data() << " \033[0m" << endl;
}

void ViLogEntry::printLong()
{
	switch(mType)
	{
		case QtDebugMsg:
			cout << "\033[1;32mStatus: ";
			break;
		case QtWarningMsg:
			cout << "\033[1;33mWarning: ";
			break;
		case QtCriticalMsg:
			cout << "\033[1;31mCritical Error: ";
			break;
        case QtFatalMsg:
			cout << "\033[1;31mFatal Error: ";
	}
	cout << "\033[1;34m" << mClassName. toLatin1().data() << "\033[1;30m (" << mFileName. toLatin1().data() << ":" << mLineNumber << ")\033[1;37m " << mMessage. toLatin1().data() << " \033[0m" << endl;
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
	mEntries.append(entry);
	if(mTerminalEnabled)
	{
		entry.print();
	}
}

void log(const char *file, const char *function, const int line, const QString message, QtMsgType type)
{
	QString fileName(file);
	fileName = fileName.mid(fileName.lastIndexOf(QDir::separator()) + 1);
	//LOGGER->append(ViLogEntry(fileName, className, QString(function), line, message, type));
}

void log(const char *file, const QString className, const char *function, const int line, const QString message, QtMsgType type)
{
	QString fileName(file);
	fileName = fileName.mid(fileName.lastIndexOf(QDir::separator()) + 1);
	LOGGER->append(ViLogEntry(fileName, className, QString(function), line, message, type));
}
