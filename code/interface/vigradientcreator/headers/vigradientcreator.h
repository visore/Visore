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
			Rectangle,
			BoldCircle ,
			Circle
		};

	public:

		static QImage createGradient(ViGradientCreator::ViGradientType type, int width, int height, QColor color);

	private:

		static QImage rectangleGradient(int width, int height, QColor color);
		static QImage radialGradient(int width, int height, QColor color);
		static QImage boldRadialGradient(int width, int height, QColor color);
		
};

#endif
