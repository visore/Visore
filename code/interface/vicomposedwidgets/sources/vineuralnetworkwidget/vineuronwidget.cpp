#include <vineuronwidget.h>
#include <QPainter>

#define DEFAULT_RADIUS 20

ViNeuronWidget::ViNeuronWidget(ViNeuron *neuron, ViWidget *parent)
	: ViWidget(parent)
{
	setPosition(0, 0);
	setRadius(DEFAULT_RADIUS);
	setNeuron(neuron);
	initailizeColors();
}

ViNeuronWidget::ViNeuronWidget(int x, int y, ViNeuron *neuron, ViWidget *parent)
	: ViWidget(parent)
{
	setPosition(x, y);
	setRadius(DEFAULT_RADIUS);
	setNeuron(neuron);
	initailizeColors();
}

ViNeuronWidget::ViNeuronWidget(int x, int y, int radius, ViNeuron *neuron, ViWidget *parent)
	: ViWidget(parent)
{
	setPosition(x, y);
	setRadius(radius);
	setNeuron(neuron);
	initailizeColors();
}

void ViNeuronWidget::setNeuron(ViNeuron *neuron)
{
	mNeuron = neuron;
}

ViNeuron* ViNeuronWidget::neuron()
{
	return mNeuron;
}

void ViNeuronWidget::setPosition(int x, int y)
{
	move(x, y);
}

void ViNeuronWidget::move(int x, int y)
{
	mX = x;
	mY = y;
	ViWidget::move(mX, mY);
	repaint();
}

void ViNeuronWidget::setX(int x)
{
	move(x, mY);
}

int ViNeuronWidget::x()
{
	return mX;
}

void ViNeuronWidget::setY(int y)
{
	move(mX, y);
}

int ViNeuronWidget::y()
{
	return mY;
}

void ViNeuronWidget::setRadius(int radius)
{
	mRadius = radius;
}

int ViNeuronWidget::radius()
{
	return mRadius;
}

QPoint ViNeuronWidget::center()
{
	return QPoint(mX + mRadius, mY + mRadius);
}

void ViNeuronWidget::initailizeColors()
{
	mBorderColor = ViThemeManager::color(ViThemeColors::BorderColor2);
	mOuterColor = ViThemeManager::color(ViThemeColors::MainColor5);
	mInnerColor = ViThemeManager::color(ViThemeColors::MainColor7);
}

void ViNeuronWidget::repaint()
{
	ViWidget::repaint();
	emit repainted();
}

void ViNeuronWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	static int borderWidth = 3;
	QPen pen(mBorderColor);
	pen.setWidth(borderWidth);
	painter.setPen(pen);

	QRadialGradient gradient(mRadius, mRadius, mRadius);
	gradient.setColorAt(0, mInnerColor);
	gradient.setColorAt(1, mOuterColor);
	painter.setBrush(QBrush(gradient));

	int size = mRadius * 2;
	setMinimumSize(size, size);
	size -= borderWidth * 2;
	painter.drawEllipse(borderWidth, borderWidth, size, size);
}
