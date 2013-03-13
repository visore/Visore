#ifndef VIBIASWIDGET_H
#define VIBIASWIDGET_H

#include <vineuronwidget.h>

class ViBiasWidget : public ViNeuronWidget
{

	public:

		ViBiasWidget(ViNeuron *neuron, ViWidget *parent = NULL);
		ViBiasWidget(int x, int y, ViNeuron *neuron, ViWidget *parent = NULL);
		ViBiasWidget(int x, int y, int radius, ViNeuron *neuron, ViWidget *parent = NULL);

	protected:

		void initailizeColors();

};

#endif
