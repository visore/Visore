#ifndef VINEURALNETWORKFACTORY_H
#define VINEURALNETWORKFACTORY_H

#include <vineuralfactory.h>
#include <vineuralnetwork.h>

class ViNeuralNetworkFactory : public ViNeuralFactory
{

	public:

		ViNeuralNetworkFactory();
		ViNeuralNetworkFactory(const ViNeuralNetworkFactory &other);
		~ViNeuralNetworkFactory();

		void clear();

		void setActivationFunction(ViActivationFunction *activationFunction); // Takes ownership of activationFunction

		void addLayer(int neuronCount);
		void addLayer(int neuronCount, double bias);
		void addLayer(int neuronCount, ViActivationFunction *activationFunction);
		void addLayer(int neuronCount, double bias, ViActivationFunction *activationFunction);

		ViNeuralNetwork* create();

		ViNeuralNetwork* create(ViElement element);
		
	private:

		ViActivationFunction *mGlobalActivationFunction;
		QList<ViActivationFunction*> mActivationFunctions;
		ViIntegerList mNeurons;
		ViDoubleList mBiases;

};

#endif
