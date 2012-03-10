#include "viwave.h"

ViWave::ViWave()
	: QList<ViAmplitude>()
{
}

ViWave::~ViWave()
{
	clear();
}
