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

QString ViLibrary::name()
{
	QString result = CLASSNAME;
	return result.remove(0, 2);
}
