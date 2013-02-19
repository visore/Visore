#ifndef VISYNAPSEFACTORY_H
#define VISYNAPSEFACTORY_H

#include <vineuralfactory.h>
#include <visynapse.h>
#include <vineuron.h>

class ViSynapseFactory : public ViNeuralFactory
{

	public:

		static ViSynapse* create(ViNeuron *input = NULL, ViNeuron *output = NULL, double weight = 1);

};

#endif
