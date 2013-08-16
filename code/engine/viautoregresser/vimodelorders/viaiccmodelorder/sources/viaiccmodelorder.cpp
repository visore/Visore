#include <viaiccmodelorder.h>

ViAiccModelOrder::ViAiccModelOrder()
	: ViAicModelOrder()
{
}

ViAiccModelOrder::ViAiccModelOrder(const ViAiccModelOrder &other)
	: ViAicModelOrder(other)
{
}

ViAiccModelOrder::~ViAiccModelOrder()
{
}

ViAiccModelOrder* ViAiccModelOrder::clone()
{
	return new ViAiccModelOrder(*this);
}

int ViAiccModelOrder::order(const int &sampleCount, const int &parameterCount, const qreal &rss)
{
	return ViAicModelOrder::order(sampleCount, parameterCount, rss) + ((2 * parameterCount * (parameterCount + 1)) / (sampleCount - parameterCount - 1));
}

#ifdef __cplusplus
extern "C"
{
#endif

ViAiccModelOrder* create()
{
	return new ViAiccModelOrder();
}

#ifdef __cplusplus
}
#endif
