#include "viwave.h"

ViWave::ViWave()
	: QList<ViAmplitude*>()
{
}

ViWave::~ViWave()
{
	for(int i = 0; i < size(); ++i)
	{
		delete (*this)[i];
		(*this)[i] = NULL;
	}
}
