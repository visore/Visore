#include <viname.h>
#include <vicommon.h>

QString ViName::name(QString replace, bool spaced)
{
	return formatName(CLASSNAME, replace, spaced);
}

QString ViName::formatName(QString name, QString replace, bool spaced)
{
	if(name.startsWith("vi", Qt::CaseInsensitive))
	{
		name = name.remove(0, 2);
	}
	name.replace(replace, "");
	if(spaced)
	{
		for(int i = 0; i < name.size(); ++i)
		{
			if(name[i].isUpper())
			{
				name.insert(i, " ");
				++i;
			}
		}
	}
	return name.trimmed();
}
