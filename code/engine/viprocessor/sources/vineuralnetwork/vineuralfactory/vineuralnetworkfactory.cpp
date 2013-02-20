#include <vineuralnetworkfactory.h>
#include <vineurallayerfactory.h>
#include <visynapsefactory.h>

ViNeuralNetworkFactory::ViNeuralNetworkFactory()
{
	mDefaultActivationFunction = NULL;
}

ViNeuralNetworkFactory::ViNeuralNetworkFactory(const ViNeuralNetworkFactory &other)
{
	mDefaultActivationFunction = other.mDefaultActivationFunction;
	mActivationFunctions = other.mActivationFunctions;
	mNeurons = other.mNeurons;
	mBiases = other.mBiases;
}

void ViNeuralNetworkFactory::clear()
{
	mDefaultActivationFunction = NULL;
	mActivationFunctions.clear();
	mNeurons.clear();
	mBiases.clear();
}

void ViNeuralNetworkFactory::setActivationFunction(ViActivationFunction *activationFunction)
{
	mDefaultActivationFunction = activationFunction;
}

void ViNeuralNetworkFactory::addLayer(int neuronCount)
{
	addLayer(neuronCount, 0, mDefaultActivationFunction);
}

void ViNeuralNetworkFactory::addLayer(int neuronCount, double bias)
{
	addLayer(neuronCount, bias, mDefaultActivationFunction);
}

void ViNeuralNetworkFactory::addLayer(int neuronCount, ViActivationFunction *activationFunction)
{
	addLayer(neuronCount, 0, activationFunction);
}

void ViNeuralNetworkFactory::addLayer(int neuronCount, double bias, ViActivationFunction *activationFunction)
{
	mActivationFunctions.append(activationFunction);
	mNeurons.append(neuronCount);
	mBiases.append(bias);
}

ViNeuralNetwork* ViNeuralNetworkFactory::create()
{
	ViNeuralNetwork *network = new ViNeuralNetwork();
	
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		if(i == 0 || i == mNeurons.size() - 1) // Don't allow biases on the input and output layer
		{
			network->add(ViNeuralLayerFactory::create(mNeurons[i], mActivationFunctions[i]));
		}
		else
		{
			network->add(ViNeuralLayerFactory::create(mNeurons[i], mActivationFunctions[i], mBiases[i]));
		}
	}

	for(int i = 0; i < network->size() - 1; ++i)
	{
		for(int j = 0; j < network[i].size() - 1; ++j)
		{
			for(int k = 0; k < network[i + 1].size() - 1; ++k)
			{
				ViSynapseFactory::create(network->at(i)->at(j), network->at(i + 1)->at(k));
			}
		}
	}

	return network;
}
