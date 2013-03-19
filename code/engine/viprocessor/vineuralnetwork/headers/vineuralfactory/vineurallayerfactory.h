#ifndef VINEURALLAYERFACTORY_H
#define VINEURALLAYERFACTORY_H

#include <vineuralfactory.h>
#include <vineurallayer.h>

class ViNeuralLayerFactory : public ViNeuralFactory
{

	public:

		static ViNeuralLayer* create(ViElement element, bool &error = STATIC_BOOLEAN_FALSE);
		static QList<ViNeuralLayer*> create(ViElementList elements, bool &error = STATIC_BOOLEAN_FALSE);

		static ViNeuralLayer* create(int neuronCount, ViNeuron::Type type, ViActivationFunction *activationFunction = NULL, double bias = 0);

		static ViNeuralLayer* createInput(int neuronCount);
		static ViNeuralLayer* createOutput(int neuronCount, ViActivationFunction *activationFunction = NULL, double bias = 0);
		static ViNeuralLayer* createHidden(int neuronCount, ViActivationFunction *activationFunction = NULL, double bias = 0);

};

#endif
