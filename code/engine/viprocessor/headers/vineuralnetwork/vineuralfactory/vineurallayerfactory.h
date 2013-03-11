#ifndef VINEURALLAYERFACTORY_H
#define VINEURALLAYERFACTORY_H

#include <vineuralfactory.h>
#include <vineurallayer.h>

class ViNeuralLayerFactory : public ViNeuralFactory
{

	public:

		static ViNeuralLayer* create(int neuronCount, ViNeuron::Type type, ViActivationFunction *activationFunction = NULL, double bias = 0);

		static ViNeuralLayer* createInput(int neuronCount);
		static ViNeuralLayer* createOutput(int neuronCount, ViActivationFunction *activationFunction, double bias = 0);
		static ViNeuralLayer* createHidden(int neuronCount, ViActivationFunction *activationFunction, double bias = 0);

};

#endif
