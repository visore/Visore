#include <vistaticneuron.h>

ViStaticNeuron::ViStaticNeuron(ViNeuron::Type type, double value)
	: ViNeuron(type)
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
