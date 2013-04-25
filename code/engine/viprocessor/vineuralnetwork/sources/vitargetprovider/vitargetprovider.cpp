#include <vitargetprovider.h>

ViTargetProvider::ViTargetProvider()
{
}

ViTargetProvider::ViTargetProvider(const ViTargetProvider &other)
{
}

ViTargetProvider::~ViTargetProvider()
{
}

ViElement ViTargetProvider::exportData()
{
	return ViElement("TargetProvider", name());
}

bool ViTargetProvider::importData(ViElement element)
{
	return element.name() == "TargetProvider" && element.value().toString() == name();
}
