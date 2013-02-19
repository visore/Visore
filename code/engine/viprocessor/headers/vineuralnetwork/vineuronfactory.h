#ifndef VINEURONFACTORY_H
#define VINEURONFACTORY_H

#include <vineuralfactory.h>
#include <vineuron.h>

class ViNeuronFactory : public ViNeuralFactory
{

	public:

		static ViNeuron* create(ViActivationFunction *activationFunction = NULL);

};

#endif
