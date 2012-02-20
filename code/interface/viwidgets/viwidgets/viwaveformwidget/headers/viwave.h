#ifndef VIWAVE_H
#define VIWAVE_H

#include <QList>
#include "viamplitude.h"

class ViWave : public QList<ViAmplitude*>
{
	public:
		ViWave();
		~ViWave();
};

#endif
