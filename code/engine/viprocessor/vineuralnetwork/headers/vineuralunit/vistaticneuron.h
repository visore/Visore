#ifndef VISTATICNEURON_H
#define VISTATICNEURON_H

#include <vineuron.h>

class ViStaticNeuronFactory;

/*
	Neuron has a value that is set externally and not by the network itself. Used for input and bias neurons.
*/

class ViStaticNeuron : public ViNeuron
{

	friend class ViStaticNeuronFactory;

	public:

		ViStaticNeuron(const ViStaticNeuron &other);

		void run();
		
	protected:

		ViStaticNeuron(ViNeuron::Type type, double value = 1);

};

#endif
