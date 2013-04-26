#include <vitargetprovider.h>

ViTargetProvider::ViTargetProvider()
{
}

ViTargetProvider::ViTargetProvider(const ViTargetProvider &other)
{
	mData = other.mData;
}

ViTargetProvider::~ViTargetProvider()
{
}

void ViTargetProvider::setData(ViSampleChunk chunk)
{
	mData = chunk;
}

const ViSampleChunk& ViTargetProvider::data()
{
	return mData;
}

ViRealList ViTargetProvider::calculate(ViRealList indexes)
{
	ViRealList results;
	for(int i = 0; i < indexes.size(); ++i)
	{
		results.append(calculate(indexes[i]));
	}
	return results;
}

ViElement ViTargetProvider::exportData()
{
	return ViElement("TargetProvider", name());
}

bool ViTargetProvider::importData(ViElement element)
{
	return element.name() == "TargetProvider" && element.value().toString() == name();
}
