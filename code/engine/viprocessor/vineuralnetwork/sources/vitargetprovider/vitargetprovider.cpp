#include <vitargetprovider.h>

ViTargetProvider::ViTargetProvider()
{
	mLeftData = NULL;
	mRightData = NULL;
}

ViTargetProvider::ViTargetProvider(const ViTargetProvider &other)
{
	mLeftData = other.mLeftData;
	mRightData = other.mRightData;
}

ViTargetProvider::~ViTargetProvider()
{
}

void ViTargetProvider::setData(ViSampleChunk *left, ViSampleChunk *right)
{
	setLeftData(left);
	setRightData(right);
}

void ViTargetProvider::setLeftData(ViSampleChunk *data)
{
	mLeftData = data;
}

void ViTargetProvider::setRightData(ViSampleChunk *data)
{
	mRightData = data;
}

ViElement ViTargetProvider::exportData()
{
	return ViElement("TargetProvider", name());
}

bool ViTargetProvider::importData(ViElement element)
{
	return element.name() == "TargetProvider" && element.value().toString() == name();
}
