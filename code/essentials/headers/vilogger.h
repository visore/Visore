#ifndef VILOGGER_H
#define VILOGGER_H

#include <QtGlobal>
#include <QList>
#include <QSharedPointer>
#include <iostream>
#include <vicommon.h>

using namespace std;

class ViLogEntry
{

	public:

		ViLogEntry(QString fileName, QString functionName, int lineNumber, QString message, QtMsgType type);
		ViLogEntry(QString fileName, QString className, QString functionName, int lineNumber, QString message, QtMsgType type);
		QString fileName();
		QString className();
		QString functionName();
		int lineNumber();
		QString message();
		QtMsgType type();
		QString toString();
		void print();

	protected:

		void printShort();
		void printMedium();
		void printLong();

	private:

		QString mFileName;
		QString mClassName;
		QString mFunctionName;
		QString mMessage;
		int mLineNumber;
		QtMsgType mType;

};

class ViLogger
{

	public:

		static ViLogger* instance();
		void enableTerminal(bool enable);
		void append(ViLogEntry entry);

	protected:

		ViLogger();

	protected:

		static QSharedPointer<ViLogger> mInstance;
		QList<ViLogEntry> mEntries;
		bool mTerminalEnabled;
	
};

#define LOG1(message) log(__FILE__, CLASSNAME, Q_FUNC_INFO, __LINE__, message, QtDebugMsg)
#define LOG2(message, type) log(__FILE__, CLASSNAME, Q_FUNC_INFO, __LINE__, message, type)

#define GET_ARGUMENT(arg1, arg2, arg3, ...) arg3
#define LOG_CHOOSER(...) GET_ARGUMENT(__VA_ARGS__, LOG2, LOG1, )

#define LOG(...) LOG_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define STATICLOG1(message) log(__FILE__, Q_FUNC_INFO, __LINE__, message, QtDebugMsg)
#define STATICLOG2(message, type) log(__FILE__, Q_FUNC_INFO, __LINE__, message, type)
#define STATICLOG3(message, type, class) log(__FILE__, class, Q_FUNC_INFO, __LINE__, message, type)

#define STATIC_GET_ARGUMENT(arg1, arg2, arg3, arg4, ...) arg4
#define STATIC_LOG_CHOOSER(...) STATIC_GET_ARGUMENT(__VA_ARGS__, STATICLOG3, STATICLOG2, STATICLOG1, )

#define STATICLOG(...) STATIC_LOG_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

ViLogger * const LOGGER = ViLogger::instance();

void log(const char *file, const char *function, const int line, const QString message, QtMsgType type);
void log(const char *file, const QString className, const char *function, const int line, const QString message, QtMsgType type);

#endif
