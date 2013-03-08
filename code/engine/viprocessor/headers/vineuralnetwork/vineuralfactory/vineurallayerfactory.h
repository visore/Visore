#ifndef VINEURALLAYERFACTORY_H
#define VINEURALLAYERFACTORY_H

#include <vineuralfactory.h>
#include <vineurallayer.h>

class ViNeuralLayerFactory : public ViNeuralFactory
{

	public:

		static ViNeuralLayer* create(int neuronCount, ViActivationFunction *activationFunction, double bias = 0);

};

#endif
