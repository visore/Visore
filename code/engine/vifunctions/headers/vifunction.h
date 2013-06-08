#ifndef VIFUNCTION_H
#define VIFUNCTION_H

#include <QtCore/qmath.h>

class ViFunction
{

	public:

		ViFunction();
		ViFunction(const ViFunction &other);
		virtual ~ViFunction();

		virtual qreal calculate(qreal x) = 0;

};

#endif
