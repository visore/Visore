#ifndef VISIGNALMANIPULATOR_H
#define VISIGNALMANIPULATOR_H

#include "viaudiobuffer.h"

class ViSignalManipulator
{

	public:

		enum Type
		{
			Tooth = 0,
			Flat = 1,
			Triangle = 2,
			Mountain = 3,
			FlatMountain = 4,
			Trapezoid = 5,
			Sin = 6,
			Cos = 7,
			Tan = 8
		};

	public:

		static void createDefaultSignal(ViAudioBuffer *buffer, qint32 cycles, Type type);

		static void createSignal(QList<qreal> samples, ViAudioBuffer *buffer);

		static void createNoise(ViAudioBuffer *input, ViAudioBuffer *output, qreal percentage);

	private:

		static void createFlatSignal(ViAudioBuffer *buffer, qint32 cycles);
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
