#include <vistaticneuronfactory.h>

ViStaticNeuron* ViStaticNeuronFactory::create(double value)
{
	return new ViStaticNeuron(value);
}
