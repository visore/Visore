#include "vimatcherstrategy.h"

ViMatcherStrategy::ViMatcherStrategy()
{
	mFirstStream = NULL;
	mSecondStream = NULL;
	mResult = NULL;
}

void ViMatcherStrategy::setBuffers(ViAudioBuffer *first, ViAudioBuffer *second)
{
	mFirstStream = first->createReadStream();
	mSecondStream = second->createReadStream();
}

void ViMatcherStrategy::setResult(ViMatchResult *result)
{
	mResult = result;
}
