#ifndef VISTATICNEURONFACTORY_H
#define VISTATICNEURONFACTORY_H

#include <vineuralfactory.h>
#include <vistaticneuron.h>

class ViStaticNeuronFactory : public ViNeuralFactory
{

	public:

		static ViStaticNeuron* create(double value = 1);

};

#endif
