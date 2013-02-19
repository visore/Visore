#include <visynapsefactory.h>

ViSynapse* ViSynapseFactory::create(ViNeuron *input, ViNeuron *output, double weight)
{
	ViSynapse *synapse = new ViSynapse(input, output, weight);
	input->addOutput(synapse);
	output->addInput(synapse);
	return synapse;
}
