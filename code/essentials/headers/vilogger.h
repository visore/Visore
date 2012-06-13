#ifndef VILOGGER_H
#define VILOGGER_H

#include <QtGlobal>
#include <QString>
#include <QList>
#include <QSharedPointer>
#include <iostream>

using namespace std;

class ViLogEntry
{

	public:

		ViLogEntry(QString className, QString functionName, int lineNumber, QString message, QtMsgType type);
		QString className();
		QString functionName();
		int lineNumber();
		QString message();
		QtMsgType type();
		QString toString();
		void print();

	private:

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

#define LOG1(message) log(__FILE__, Q_FUNC_INFO, __LINE__, message, QtDebugMsg)
#define LOG2(message, type) log(__FILE__, Q_FUNC_INFO, __LINE__, message, type)

#define GET_ARGUMENT(arg1, arg2, arg3, ...) arg3
#define LOG_CHOOSER(...) GET_ARGUMENT(__VA_ARGS__, LOG2, LOG1, )

#define LOG(...) LOG_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

ViLogger * const LOGGER = ViLogger::instance();

void log(const char *file, const char *function, const int line, const QString message, QtMsgType type);

#endif
