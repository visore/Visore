#ifndef VIRANDOMGENERATOR_H
#define VIRANDOMGENERATOR_H

#include <stdlib.h>
#include <time.h>
#include <viscaler.h>
#include <QtGlobal>

#include<vilogger.h>

class ViRandomGenerator
{
	public:

		static qreal generate(qreal lowerLimit, qreal upperLimit);
		static qreal generate(int lowerLimit, qreal upperLimit);
		static qreal generate(qreal lowerLimit, int upperLimit);
		static int generate(int lowerLimit, int upperLimit);

	private:

		static bool initialize();

};

#endif
