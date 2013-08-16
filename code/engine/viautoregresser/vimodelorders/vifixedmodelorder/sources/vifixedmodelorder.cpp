#include <vifixedmodelorder.h>

#define DEFAULT_ORDER 8

ViFixedModelOrder::ViFixedModelOrder()
	: ViModelOrder()
{
	mOrder = DEFAULT_ORDER;
}

ViFixedModelOrder::ViFixedModelOrder(const ViFixedModelOrder &other)
	: ViModelOrder(other)
{
	mOrder = other.mOrder;
}

ViFixedModelOrder::~ViFixedModelOrder()
{
}

ViFixedModelOrder* ViFixedModelOrder::clone()
{
	return new ViFixedModelOrder(*this);
}

void ViFixedModelOrder::clear()
{
	ViModelOrder::clear();
	mOrder = DEFAULT_ORDER;
}

void ViFixedModelOrder::setOrder(const int &order)
{
	mOrder = order;
}

int ViFixedModelOrder::order() const
{
	return mOrder;
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFixedModelOrder* create()
{
	return new ViFixedModelOrder();
}

#ifdef __cplusplus
}
#endif
