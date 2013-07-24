#include <vitargetprovider.h>

ViTargetProvider::ViTargetProvider(int leftSamples, int rightSamples)
{
	mLeftData = NULL;
	mRightData = NULL;
	setLeftSamples(leftSamples);
	setRightSamples(rightSamples);
}

ViTargetProvider::ViTargetProvider(const ViTargetProvider &other)
{
	mLeftData = other.mLeftData;
	mRightData = other.mRightData;
	mLeftSamples = other.mLeftSamples;
	mRightSamples = other.mRightSamples;
}

ViTargetProvider::~ViTargetProvider()
{
}

int ViTargetProvider::leftSamples()
{
	return mLeftSamples;
}

int ViTargetProvider::rightSamples()
{
	return mRightSamples;
}

void ViTargetProvider::setLeftSamples(int samples)
{
	mLeftSamples = samples;
}

void ViTargetProvider::setRightSamples(int samples)
{
	mRightSamples = samples;
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
	ViElement root("targetprovider");
	root.addChild(ViElement("name", name("TargetProvider")));
    return root;
}

bool ViTargetProvider::importData(ViElement element)
{
	return element.name() == "targetprovider" && element.value().toString() == name("TargetProvider");
}
