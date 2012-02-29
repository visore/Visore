#ifndef VIGRADIENTCREATOR_H
#define VIGRADIENTCREATOR_H

#include <QRadialGradient>
#include <QLinearGradient>
#include <QPainter>
#include <qmath.h>
#include "vithememanager.h"

class ViGradientCreator
{
	public:
		enum ViGradientType
		{
			Rectangle = 0,
			Circle = 1
		};
		static QImage createGradient(ViGradientCreator::ViGradientType type, int width, int height);

	private:
		static QImage rectangleGradient(int width, int height);
		static QImage radialGradient(int width, int height);
		
};

#endif
