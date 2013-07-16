#include <viname.h>
#include <vicommon.h>

QString ViName::name(QString replace, bool spaced)
{
	QString result = CLASSNAME.remove(0, 2);
	result.replace(replace, "");
	if(spaced)
	{
		for(int i = 0; i < result.size(); ++i)
		{
			if(result[i].isUpper())
			{
				result.insert(i, " ");
				++i;
			}
		}
	}
	return result.trimmed();
}
