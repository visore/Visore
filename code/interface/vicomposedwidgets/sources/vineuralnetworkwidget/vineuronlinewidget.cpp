#include <vineuronlinewidget.h>
#include <QPainter>
#include <QPen>
#include <QPolygon>

#define DEFAULT_LENGTH 60
#define DEFAULT_TICKNESS 3

#define ARROW_WIDTH 24
#define ARROW_HEIGHT ARROW_WIDTH / 2

ViNeuronLineWidget::ViNeuronLineWidget(ViWidget *parent)
	: ViWidget(parent)
{
	mNeuronWidget = NULL;
	setLength(DEFAULT_LENGTH);
	setThickness(DEFAULT_TICKNESS);
	initailizeColors();
}

ViNeuronLineWidget::ViNeuronLineWidget(ViNeuronWidget *neuronWidget, ViWidget *parent)
	: ViWidget(parent)
{
	mNeuronWidget = NULL;
	setNeuronWidget(neuronWidget);
	setLength(DEFAULT_LENGTH);
	setThickness(DEFAULT_TICKNESS);
	initailizeColors();
}

ViNeuronLineWidget::ViNeuronLineWidget(ViNeuronWidget *neuronWidget, int length, int thickness, ViWidget *parent)
	: ViWidget(parent)
{
	mNeuronWidget = NULL;
	setNeuronWidget(neuronWidget);
	setLength(length);
	setThickness(thickness);
	initailizeColors();
}
		
void ViNeuronLineWidget::setLength(int length)
{
	if(length < 0)
	{
		mLength = 0;
	}
	else
	{
		mLength = length;
	}
}

int ViNeuronLineWidget::length()
{
	return mLength;
}

void ViNeuronLineWidget::setThickness(int thickness)
{
	mThickness = thickness;
}

int ViNeuronLineWidget::thickness()
{
	return mThickness;
}

void ViNeuronLineWidget::setNeuronWidget(ViNeuronWidget *neuronWidget)
{
	if(mNeuronWidget != NULL)
	{
		QObject::disconnect(mNeuronWidget, SIGNAL(repainted()), this, SLOT(repaint()));
	}
	mNeuronWidget = neuronWidget;
	if(mNeuronWidget != NULL)
	{
		QObject::connect(mNeuronWidget, SIGNAL(repainted()), this, SLOT(repaint()), Qt::DirectConnection);
	}
}

ViNeuronWidget* ViNeuronLineWidget::neuronWidget()
{
	return mNeuronWidget;
}

void ViNeuronLineWidget::initailizeColors()
{
	mColor = ViThemeManager::color(ViThemeColors::BorderColor1);
}

void ViNeuronLineWidget::repaint()
{
	if(mNeuronWidget != NULL)
	{
		QPoint center = mNeuronWidget->center();

		if(mNeuronWidget->neuron()->type() == ViNeuron::InputNeuron)
		{
			move(center.x() - mNeuronWidget->radius() - mLength + mThickness, center.y() - ARROW_HEIGHT);
		}
		else if(mNeuronWidget->neuron()->type() == ViNeuron::OutputNeuron)
		{
			move(center.x() + mNeuronWidget->radius() - mThickness, center.y() - ARROW_HEIGHT);
		}

		setSize(mLength, 2 * ARROW_HEIGHT);
	}
	
	ViWidget::repaint();
}

void ViNeuronLineWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(mColor);
	pen.setWidth(mThickness);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	painter.setPen(pen);
	
	painter.drawLine(0, ARROW_HEIGHT, width() - mThickness, ARROW_HEIGHT);

	QPolygon polygon;
	polygon << QPoint(width() - ARROW_WIDTH, mThickness) << QPoint(width() - mThickness, ARROW_HEIGHT) << QPoint(width() - ARROW_WIDTH, height() - mThickness);
	
	painter.drawPolyline(polygon);
}
