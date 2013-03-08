#include "vifunctor.h"

void ViFunctor::execute(void *data)
{
}

void ViFunctor::operator () (ViFunctorParameter *data)
{
	execute(data);
}
