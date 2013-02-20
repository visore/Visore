#include <vineuronfactory.h>

ViNeuron* ViNeuronFactory::create(ViActivationFunction *activationFunction)
{
	return new ViNeuron(activationFunction);
}
