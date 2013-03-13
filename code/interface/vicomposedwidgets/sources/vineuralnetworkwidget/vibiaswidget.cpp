#include <vibiaswidget.h>

ViBiasWidget::ViBiasWidget(ViNeuron *neuron, ViWidget *parent)
	: ViNeuronWidget(neuron, parent)
{
}

ViBiasWidget::ViBiasWidget(int x, int y, ViNeuron *neuron, ViWidget *parent)
	: ViNeuronWidget(x, y, neuron, parent)
{
}

ViBiasWidget::ViBiasWidget(int x, int y, int radius, ViNeuron *neuron, ViWidget *parent)
	: ViNeuronWidget(x, y, radius, neuron, parent)
{
}

void ViBiasWidget::initailizeColors()
{
	mBorderColor = ViThemeManager::color(ViThemeColors::BorderColor2);
	mOuterColor = ViThemeManager::color(ViThemeColors::MainColor7);
	mInnerColor = ViThemeManager::color(ViThemeColors::MainColor5);
}
