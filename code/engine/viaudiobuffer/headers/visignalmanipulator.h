#ifndef VISIGNALMANIPULATOR_H
#define VISIGNALMANIPULATOR_H

#include "viaudiobuffer.h"

class ViSignalManipulator
{

	public:

		static void createSignal(QList<qreal> samples, ViAudioBuffer *buffer);
		static void addRandomNoise(ViAudioBuffer *buffer, qreal percentage);
};

#endif
