#include <viinterpolationtargetprovider.h>

ViInterpolationTargetProvider::ViInterpolationTargetProvider()
	: ViTargetProvider()
{
	mInterpolator = NULL;
}

ViInterpolationTargetProvider::ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other)
	: ViTargetProvider(other)
{
	if(other.mInterpolator == NULL)
	{
		mInterpolator = NULL;	
	}
	else
	{
		mInterpolator = other.mInterpolator->clone();
	}
}

ViInterpolationTargetProvider::~ViInterpolationTargetProvider()
{
	if(mInterpolator != NULL)
	{
		delete mInterpolator;
		mInterpolator = NULL;
	}
}

void ViInterpolationTargetProvider::setInterpolator(ViInterpolator *interpolator)
{
	if(mInterpolator != NULL)
	{
		delete mInterpolator;
	}
	mInterpolator = interpolator;
}

qreal ViInterpolationTargetProvider::calculate()
{
	mInterpolator->setData(mLeftData, mRightData);
	return mInterpolator->calculate();
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
