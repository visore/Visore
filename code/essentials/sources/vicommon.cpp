#include <vicommon.h>

QString stripClassName(const char *text)
{
	QString name(text);
	if(name.startsWith("P") && name[1].isDigit())
	{
		name.remove(0, 1);
	}
	while(name.size() > 0 && name[0].isDigit())
	{
		name.remove(0, 1);
	}
	return name;
}
