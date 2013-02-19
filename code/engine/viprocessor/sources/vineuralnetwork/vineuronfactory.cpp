#include <vineuronfactory.h>

ViNeuron* ViNeuronFactory::create(ViActivationFunction *activationFunction)
{
	ViNeuron *neuron = new ViNeuron(activationFunction);
	return neuron;
}
