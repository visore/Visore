#include <vineuronwidget.h>
#include <QPainter>

ViNeuronWidget::ViNeuronWidget(ViWidget *parent)
	: ViWidget(parent)
{
	setX(0);
	setY(0);
	mRadius = 0;
	initailizeColors();
}

ViNeuronWidget::ViNeuronWidget(int x, int y, ViWidget *parent)
	: ViWidget(parent)
{
	setX(x);
	setY(y);
	mRadius = 0;
	initailizeColors();
}

ViNeuronWidget::ViNeuronWidget(int x, int y, int radius, ViWidget *parent)
	: ViWidget(parent)
{
	setX(x);
	setY(y);
	mRadius = radius;
	initailizeColors();
}
		
void ViNeuronWidget::setPosition(int x, int y)
{
	setX(x);
	setY(y);
}

void ViNeuronWidget::setX(int x)
{
	mX = x;
	move(mX, mY);
}

int ViNeuronWidget::x()
{
	return mX;
}

void ViNeuronWidget::setY(int y)
{
	mY = y;
	move(mX, mY);
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

void ViNeuronWidget::initailizeColors()
{
	mBorderColor = ViThemeManager::color(ViThemeColors::BorderColor1);
	mOuterColor = ViThemeManager::color(ViThemeColors::MainColor5);
	mInnerColor = ViThemeManager::color(ViThemeColors::MainColor7);
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
