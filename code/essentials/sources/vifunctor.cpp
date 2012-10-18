#include "vifunctor.h"

void ViFunctor::operator () (ViFunctorParameter *data)
{
	execute(data);
}
