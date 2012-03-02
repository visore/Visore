#include "viwindowfunction.h"

QSharedPointer<ViWindowFunction> ViWindowFunction::mInstance;

ViWindowFunction::ViWindowFunction()
	: ViSingleton()
{
}

ViWindowFunction* ViWindowFunction::instance()
{
	return NULL;
}
