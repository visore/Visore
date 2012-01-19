#include "vilogger.h"

void ViLogger::debug(QString message)
{
	cout << message.toUtf8().data() << endl;
}
