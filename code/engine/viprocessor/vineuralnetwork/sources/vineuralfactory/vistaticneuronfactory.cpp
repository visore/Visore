#include <vistaticneuronfactory.h>

ViStaticNeuron* ViStaticNeuronFactory::create(ViNeuron::Type type, double value)
{
	return new ViStaticNeuron(type, value);
}
