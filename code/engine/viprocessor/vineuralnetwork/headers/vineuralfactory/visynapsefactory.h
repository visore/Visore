#ifndef VISYNAPSEFACTORY_H
#define VISYNAPSEFACTORY_H

#include <vineuralfactory.h>
#include <visynapse.h>
#include <vineuron.h>

class ViNeuralNetwork;

class ViSynapseFactory : public ViNeuralFactory
{

	public:

		static ViSynapse* create(ViElement element, ViNeuralNetwork *network, bool &error = STATIC_BOOLEAN_FALSE);
		static QList<ViSynapse*> create(ViElementList elements, ViNeuralNetwork *network, bool &error = STATIC_BOOLEAN_FALSE);

		static ViSynapse* create(ViNeuron *input = NULL, ViNeuron *output = NULL, double weight = 1);

};

#endif
