#include <vineuronfactory.h>

ViNeuron* ViNeuronFactory::create(ViNeuron::Type type, ViActivationFunction *activationFunction)
{
	if(activationFunction == NULL)
	{
		return new ViNeuron(type);
	}
	return new ViNeuron(type, activationFunction->clone());
}
