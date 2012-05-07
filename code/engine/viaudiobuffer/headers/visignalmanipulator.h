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
			FlatMountain = 3,
			Trapezoid = 4,
			Sin = 5,
			Cos = 6,
			Tan = 7
		};

	public:

		static void createDefaultSignal(ViAudioBuffer *buffer, qint32 cycles, Type type);

		static void createSignal(QList<qreal> samples, ViAudioBuffer *buffer);

		static void createNoise(ViAudioBuffer *input, ViAudioBuffer *output, qreal percentage);

	private:

		static void createToothSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createTriangleSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createMountainSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createFlatMountainSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createTrapezoidSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createSinSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createCosSignal(ViAudioBuffer *buffer, qint32 cycles);
		static void createTanSignal(ViAudioBuffer *buffer, qint32 cycles);

};

#endif
