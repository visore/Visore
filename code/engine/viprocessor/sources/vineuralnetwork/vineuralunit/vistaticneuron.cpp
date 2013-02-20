#include <vistaticneuron.h>

ViStaticNeuron::ViStaticNeuron(double value)
	: ViNeuron()
{
	setValue(value);
}

ViStaticNeuron::ViStaticNeuron(const ViStaticNeuron &other)
	: ViNeuron(other)
{
}

void ViStaticNeuron::run()
{
}
