#ifndef VILOGGER_H
#define VILOGGER_H

#include <QString>
#include <iostream>

using namespace std;

class ViLogger
{
	public:
		static void debug(QString message)
		{
			#ifdef DEBUG
				cout << "ViLogger debug: " << message.toUtf8().data() << endl;
			#endif
		}
};

#endif
