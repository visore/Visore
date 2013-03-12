#include <vineuronfactory.h>

ViNeuron* ViNeuronFactory::create(const ViElement &element, bool &error)
{
	ViNeuron* neuron = new ViNeuron();
	if(!neuron->importData(element))
	{
		error = true;
		delete neuron;
		neuron = NULL;
	}
	return neuron;
}

ViNeuron* ViNeuronFactory::create(ViNeuron::Type type, ViActivationFunction *activationFunction)
{
	if(activationFunction == NULL)
	{
		return new ViNeuron(type);
	}
	return new ViNeuron(type, activationFunction->clone());
}
