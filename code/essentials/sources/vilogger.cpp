#include "vilogger.h"
#include <QDir>

ViLogEntry::ViLogEntry(QString className, QString functionName, int lineNumber, QString message, QtMsgType type)
{
	mClassName = className;
	mFunctionName = functionName;
	mMessage = message;
	mLineNumber = lineNumber;
	mType = type;
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
	result += mFunctionName + "(" + mClassName + ":" + QString::number(mLineNumber) + ") " + mMessage;
	return result;
}

void ViLogEntry::print()
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
	cout << "\033[1;34m" << mFunctionName.toAscii().data() << " \033[1;30m(" << mClassName.toAscii().data() << ":" << mLineNumber << ")\033[1;37m " << mMessage.toAscii().data() << " \033[0m" << endl;
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

void log(const char *file, const char *function, const int line, const char *message, QtMsgType type)
{
	QString fileName(file);
	fileName = fileName.mid(fileName.lastIndexOf(QDir::separator()) + 1);
	LOGGER->append(ViLogEntry(fileName, QString(function), line, QString(message), type));
}
