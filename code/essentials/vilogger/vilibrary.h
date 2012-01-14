#ifndef VILOGGER_H
#define VILOGGER_H

#include <QString>
#include <iostream>

using namespace std;

class ViLogger
{
	public:
		static void debug(QString message);
}

#endif

void ViLogger::debug(QString message)
{
	cout << "Visore Logger: " << message.toUtf8().data() << endl;
}
