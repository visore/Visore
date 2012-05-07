#ifndef VISIGNALMANIPULATOR_H
#define VISIGNALMANIPULATOR_H

#include "viaudiobuffer.h"

class ViSignalManipulator
{

	public:

		enum Type
		{
			Tooth = 0,
			Triangle = 1,
			Mountain = 2,
			Trapezoid = 3
		};

	public:

		static void createDefaultSignal(ViAudioBuffer *buffer, qint32 cycles, Type type);

		static void createSignal(QList<qreal> samples, ViAudioBuffer *buffer);

		static void createNoise(ViAudioBuffer *input, ViAudioBuffer *output, qreal percentage);

	private:

		static void createToothSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createTriangleSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createMountainSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createTrapezoidSignal(ViAudioBuffer *buffer, qint32 cycles);

};

#endif
