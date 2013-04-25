#include <viinterpolationtargetprovider.h>

ViInterpolationTargetProvider::ViInterpolationTargetProvider()
	: ViTargetProvider()
{
}

ViInterpolationTargetProvider::ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other)
	: ViTargetProvider(other)
{
	mChunk = other.mChunk;
}

ViInterpolationTargetProvider::~ViInterpolationTargetProvider()
{
}

void ViInterpolationTargetProvider::setData(ViSampleChunk chunk)
{
	mChunk = chunk;
}

qreal ViInterpolationTargetProvider::calculate()
{

}

ViElement ViInterpolationTargetProvider::exportData()
{
	
}

bool ViInterpolationTargetProvider::importData(ViElement element)
{

}

ViInterpolationTargetProvider* ViInterpolationTargetProvider::clone()
{
	return new ViInterpolationTargetProvider(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViTargetProvider* create()
{
	return new ViInterpolationTargetProvider();
}

#ifdef __cplusplus
}
#endif
