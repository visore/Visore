#include <viinterpolationcorrector.h>

ViInterpolationCorrector::ViInterpolationCorrector(ViInterpolator *interpolator)
	: ViModifyProcessor()
{
	mInterpolator = interpolator;
}

ViInterpolationCorrector::~ViInterpolationCorrector()
{
	clear();
}

void ViInterpolationCorrector::clear()
{
	if(mInterpolator == NULL)
	{
		delete mInterpolator;
		mInterpolator = NULL;
	}
	ViModifyProcessor::clear();
}

void ViInterpolationCorrector::setInterpolator(ViInterpolator *interpolator)
{
	clear();
	mInterpolator = interpolator;
}

void ViInterpolationCorrector::initialize()
{
	if(mInterpolator == NULL)
	{
		LOG("No interpolator specified.", QtCriticalMsg);
	}
}

void ViInterpolationCorrector::execute(const int &channel)
{
	/*ViSampleChunk &chunk = currentSamples(channel);
	mInterpolator->interpolate(chunk, currentNoise(channel));
	write(chunk, channel);*/
}

void ViInterpolationCorrector::finalize()
{

}

ViElement ViInterpolationCorrector::exportData()
{
	ViElement root = ViModifyProcessor::exportData();
	root.prependChild("name", name());
	if(mInterpolator != NULL)
	{
		root.addChild(mInterpolator->exportData());
	}
	return root;
}

bool ViInterpolationCorrector::importData(ViElement element)
{
	return ViModifyProcessor::importData(element) && element.child("name").toString() == name();
}
