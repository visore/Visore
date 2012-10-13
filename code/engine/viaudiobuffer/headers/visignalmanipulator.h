#ifndef VISIGNALMANIPULATOR_H
#define VISIGNALMANIPULATOR_H

#include "vibuffer.h"

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

		static void createDefaultSignal(ViBuffer *buffer, qint32 cycles, Type type);

		static void createSignal(QList<qreal> samples, ViBuffer *buffer);

		static void createNoise(ViBuffer *input, ViBuffer *output, qreal percentage);

	private:

		static void createFlatSignal(ViBuffer *buffer, qint32 cycles);
		static void createToothSignal(ViBuffer *buffer, qint32 cycles);
		static void createTriangleSignal(ViBuffer *buffer, qint32 cycles);
		static void createMountainSignal(ViBuffer *buffer, qint32 cycles);
		static void createFlatMountainSignal(ViBuffer *buffer, qint32 cycles);
		static void createTrapezoidSignal(ViBuffer *buffer, qint32 cycles);
		static void createSinSignal(ViBuffer *buffer, qint32 cycles);
		static void createCosSignal(ViBuffer *buffer, qint32 cycles);
		static void createTanSignal(ViBuffer *buffer, qint32 cycles);

};

#endif
