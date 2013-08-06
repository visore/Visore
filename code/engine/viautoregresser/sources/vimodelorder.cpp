#include <vimodelorder.h>

ViModelOrder::ViModelOrder()
{
	mSampleCount = 0;
	mParameterCount = 0;
}

ViModelOrder::ViModelOrder(const ViModelOrder &other)
{
	mSampleCount = other.mSampleCount;
	mParameterCount = other.mParameterCount;
}

ViModelOrder::~ViModelOrder()
{
}

void ViModelOrder::clear()
{
	mSampleCount = 0;
	mParameterCount = 0;
}

void ViModelOrder::setParameterCount(const int &parameters)
{
	mParameterCount = parameters;
}

void ViModelOrder::setSampleCount(const int &count)
{
	mSampleCount = count;
}

/***************************************************
	MANAGER
***************************************************/

QString ViModelOrderManager::libraryPath()
{
	return MODEL_ORDER_PATH;
}

QString ViModelOrderManager::defaultLibrary()
{
	return "MazzoniModelOrder";
}
