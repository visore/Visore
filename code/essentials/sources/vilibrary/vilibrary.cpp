#include <vilibrary.h>

ViLibrary::ViLibrary()
{
}

ViLibrary::ViLibrary(const ViLibrary &other)
{
}

ViLibrary::~ViLibrary()
{
}

QString ViLibrary::name(QString replace, bool spaced)
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

ViLibrary* ViLibrary::clone()
{
    return NULL;
}
