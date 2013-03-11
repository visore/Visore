#include <vineuronfactory.h>

ViNeuron* ViNeuronFactory::create(ViNeuron::Type type, ViActivationFunction *activationFunction)
{
	return new ViNeuron(type, activationFunction->clone());
}
