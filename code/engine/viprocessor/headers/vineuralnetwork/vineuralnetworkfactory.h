#ifndef VINEURALNETWORKFACTORY_H
#define VINEURALNETWORKFACTORY_H

#include <vineuralfactory.h>
#include <vineuralnetwork.h>

class ViNeuralNetworkFactory : public ViNeuralFactory
{

	public:

		static ViNeuralNetwork* create(int layers, int neurons, ViActivationFunction *activationFunction = NULL);
		static ViNeuralNetwork* create(ViIntegerList layers, ViActivationFunction *activationFunction = NULL);

};

#endif
