#ifndef VINEURONFACTORY_H
#define VINEURONFACTORY_H

#include <vineuralfactory.h>
#include <vineuron.h>

class ViNeuronFactory : public ViNeuralFactory
{

	public:

		static ViNeuron* create(const ViElement &element, bool &error = STATIC_BOOLEAN_FALSE);
		static ViNeuron* create(ViNeuron::Type type, ViActivationFunction *activationFunction = NULL);

};

#endif
