#include "vigradientcreator.h"

QImage ViGradientCreator::createGradient(ViGradientCreator::ViGradientType type, int width, int height, QColor color)
{
	if(type == ViGradientCreator::Rectangle)
	{
		return rectangleGradient(width, height, color);
	}
	else if(type == ViGradientCreator::BoldCircle)
	{
		return boldRadialGradient(width, height, color);
	}
	else if(type == ViGradientCreator::Circle)
	{
		return radialGradient(width, height, color);
	}
}

QImage ViGradientCreator::rectangleGradient(int width, int height, QColor color)
{
	qreal halfHeight = height / 2;
	qreal halfWidth = width / 2;
	//A small change is needed, otherwise Qt make transparent to white
	qreal halfWidthChange = halfWidth - 0.00001;

	QImage::Format format = QImage::Format_ARGB32;
    QImage buffer(qCeil(width), qCeil(height), format);
	buffer.fill(qRgba(255, 255, 255, 0));

	QPainter painter(&buffer);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor newColor = color;
	newColor.setAlpha(150);
	QGradient gradient;
	gradient.setColorAt(1, Qt::transparent);
	gradient.setColorAt(0.8, color);
	gradient.setColorAt(0.7, newColor);
	gradient.setColorAt(0, newColor);

	QLinearGradient linearGradient;

	QPainterPath clip;
	clip.moveTo(0, 0);
	clip.lineTo(width, 0);
	clip.lineTo(0, height);
	clip.lineTo(width, height);
	clip.closeSubpath();
	painter.setClipPath(clip);

	linearGradient = QLinearGradient(0, 0, width, halfHeight);
	linearGradient.setStops(gradient.stops());
	linearGradient.setStart(halfWidth, halfHeight);
	linearGradient.setFinalStop(halfWidthChange, 0);
	painter.setBrush(linearGradient);
	painter.drawRect(0, 0, width, halfHeight);

	linearGradient = QLinearGradient(0, 0, width, halfHeight);
	linearGradient.setStops(gradient.stops());
	linearGradient.setStart(halfWidth, halfHeight);
	linearGradient.setFinalStop(halfWidthChange, height);
	painter.setBrush(linearGradient);
	painter.drawRect(0, halfHeight, width, halfHeight);

	QPainterPath clip2;
	clip2.moveTo(width, 0);
	clip2.lineTo(width, height);
	clip2.lineTo(0, 0);
	clip2.lineTo(0, height);
	clip2.closeSubpath();
	painter.setClipPath(clip2);

	linearGradient = QLinearGradient(0, 0, halfWidth, height);
	linearGradient.setStops(gradient.stops());
	linearGradient.setStart(halfWidth, halfHeight);
	linearGradient.setFinalStop(width, halfHeight);
	painter.setBrush(linearGradient);
	painter.drawRect(halfWidth, 0, width, height);

	linearGradient = QLinearGradient(0, 0, halfWidth, height);
	linearGradient.setStops(gradient.stops());
	linearGradient.setStart(halfWidth, halfHeight);
	linearGradient.setFinalStop(0, halfHeight);
	painter.setBrush(linearGradient);
	painter.drawRect(0, 0, halfWidth, height);

	painter.end();
    return buffer;
}

QImage ViGradientCreator::radialGradient(int width, int height, QColor color)
{
	QImage::Format format = QImage::Format_ARGB32;
    QImage buffer(qCeil(width), qCeil(height), format);
	buffer.fill(qRgba(255, 255, 255, 0));

	QPainter painter(&buffer);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QRadialGradient radialGradient(QPointF(width / 2, height /2), qMax(width, height) / 2);
	QColor newColor = color;
	newColor.setAlpha(150);
	radialGradient.setColorAt(1, Qt::transparent);
	radialGradient.setColorAt(0, newColor);

	painter.setBrush(radialGradient);
	painter.drawRect(0, 0, width, height);

	painter.end();
    return buffer;
}

QImage ViGradientCreator::boldRadialGradient(int width, int height, QColor color)
{
	QImage::Format format = QImage::Format_ARGB32;
    QImage buffer(qCeil(width), qCeil(height), format);
	buffer.fill(qRgba(255, 255, 255, 0));

	QPainter painter(&buffer);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QRadialGradient radialGradient(QPointF(width / 2, height /2), qMax(width, height) / 2);
	QColor newColor = color;
	newColor.setAlpha(150);
	radialGradient.setColorAt(1, Qt::transparent);
	radialGradient.setColorAt(0.8, color);
	radialGradient.setColorAt(0.7, newColor);
	radialGradient.setColorAt(0, newColor);

	painter.setBrush(radialGradient);
	painter.drawRect(0, 0, width, height);

	painter.end();
    return buffer;
}
