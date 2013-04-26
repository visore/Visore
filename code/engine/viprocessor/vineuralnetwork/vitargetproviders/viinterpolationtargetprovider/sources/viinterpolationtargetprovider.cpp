#include <viinterpolationtargetprovider.h>

ViInterpolationTargetProvider::ViInterpolationTargetProvider()
	: ViTargetProvider()
{
}

ViInterpolationTargetProvider::ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other)
	: ViTargetProvider(other)
{
}

ViInterpolationTargetProvider::~ViInterpolationTargetProvider()
{
}

qreal ViInterpolationTargetProvider::calculate(int index)
{
	if(index < 0)
	{
		index = data().size() / 2;
	}
	return (data()[0] + data()[data().size()-1]) / 2;
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
