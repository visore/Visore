#include <visynapsewidget.h>
#include <QPainter>
#include <QPen>

#define DEFAULT_TICKNESS 3
#define WEIGHT_OFFSET_PERCENTAGE 0.2 // How far from the start the weight should show
#define WEIGHT_WIDTH 20

ViSynapseWidget::ViSynapseWidget(ViSynapse *synapse, ViWidget *parent)
	: ViWidget(parent)
{
	mFromX = 0;
	mFromY = 0;
	mToX = 0;
	mToY = 0;
	mInputWidget = NULL;
	mOutputWidget = NULL;
	setThickness(DEFAULT_TICKNESS);
	setSynapse(synapse);
	initailizeColors();
}

ViSynapseWidget::ViSynapseWidget(ViNeuronWidget *inputWidget, ViNeuronWidget *outputWidget, ViSynapse *synapse, ViWidget *parent)
	: ViWidget(parent)
{
	mFromX = 0;
	mFromY = 0;
	mToX = 0;
	mToY = 0;
	mInputWidget = NULL;
	mOutputWidget = NULL;
	setWidgets(inputWidget, outputWidget);
	setThickness(DEFAULT_TICKNESS);
	setSynapse(synapse);
	initailizeColors();
}

void ViSynapseWidget::setSynapse(ViSynapse *synapse)
{
	mSynapse = synapse;
}

ViSynapse* ViSynapseWidget::synapse()
{
	return mSynapse;
}
		
void ViSynapseWidget::setWidgets(ViNeuronWidget *inputWidget, ViNeuronWidget *outputWidget)
{
	setInputWidget(inputWidget);
	setOutputWidget(outputWidget);
}

void ViSynapseWidget::setInputWidget(ViNeuronWidget *widget)
{
	if(mInputWidget != NULL)
	{
		QObject::disconnect(mInputWidget, SIGNAL(repainted()), this, SLOT(repaint()));
	}
	mInputWidget = widget;
	if(mInputWidget != NULL)
	{
		QObject::connect(mInputWidget, SIGNAL(repainted()), this, SLOT(repaint()), Qt::DirectConnection);
	}
}

void ViSynapseWidget::setOutputWidget(ViNeuronWidget *widget)
{
	if(mOutputWidget != NULL)
	{
		QObject::disconnect(mOutputWidget, SIGNAL(repainted()), this, SLOT(repaint()));
	}
	mOutputWidget = widget;
	if(mOutputWidget != NULL)
	{
		QObject::connect(mOutputWidget, SIGNAL(repainted()), this, SLOT(repaint()), Qt::DirectConnection);
	}
}

ViNeuronWidget* ViSynapseWidget::inputWidget()
{
	return mInputWidget;
}

ViNeuronWidget* ViSynapseWidget::outputWidget()
{
	return mOutputWidget;
}

void ViSynapseWidget::setThickness(int thickness)
{
	mThickness = thickness;
}

int ViSynapseWidget::thickness()
{
	return mThickness;
}

void ViSynapseWidget::initailizeColors()
{
	mColor = ViThemeManager::color(ViThemeColors::BorderColor2);
}

void ViSynapseWidget::repaint()
{
	if(mInputWidget != NULL && mOutputWidget != NULL)
	{
		int minX = qMin(mInputWidget->x(), mOutputWidget->x());
		int minY = qMin(mInputWidget->y(), mOutputWidget->y());

		mFromX = mInputWidget->x() - minX;
		mFromY = mInputWidget->y() - minY;
		mToX = mOutputWidget->x() - minX;
		mToY = mOutputWidget->y() - minY;

		move(minX + mInputWidget->radius(), minY + mInputWidget->radius());
		setSize(qMax(mThickness, qAbs(mInputWidget->x() - mOutputWidget->x())), qMax(mThickness, qAbs(mInputWidget->y() - mOutputWidget->y())));
		//move(minX + mInputWidget->radius(), minY + mInputWidget->radius() - WEIGHT_WIDTH);
		//setSize(qMax(mThickness, qAbs(mInputWidget->x() - mOutputWidget->x())), qMax(mThickness, qAbs(mInputWidget->y() - mOutputWidget->y())) + (2 * WEIGHT_WIDTH));
	}
	
	ViWidget::repaint();
}

void ViSynapseWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(mColor);
	if(height() == mThickness)
	{
		pen.setWidth(mThickness * 1.8); //Mke straight line thicker, since they seem thinner
	}
	else
	{
		pen.setWidth(mThickness);
	}
	painter.setPen(pen);
	painter.setBrush(mColor);

	painter.drawLine(mFromX, mFromY, mToX, mToY);

	int weightX = (qAbs(mFromX - mToX) * WEIGHT_OFFSET_PERCENTAGE);
	int weightY = mFromY - ((mFromY - mToY + (WEIGHT_WIDTH/2)) * WEIGHT_OFFSET_PERCENTAGE);
	painter.drawEllipse(weightX, weightY, WEIGHT_WIDTH, WEIGHT_WIDTH);

	/*painter.drawLine(mFromX, mFromY + WEIGHT_WIDTH, mToX, mToY + WEIGHT_WIDTH);

	int weightX = (qAbs(mFromX - mToX) * WEIGHT_OFFSET_PERCENTAGE);
	int weightY = mFromY+ WEIGHT_WIDTH - ((mFromY - mToY + WEIGHT_WIDTH) * WEIGHT_OFFSET_PERCENTAGE);
	painter.drawEllipse(weightX, weightY, WEIGHT_WIDTH, WEIGHT_WIDTH);*/

}
