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

ViNeuralNetworkFactory::~ViNeuralNetworkFactory()
{
	clear();
}

void ViNeuralNetworkFactory::clear()
{
	if(mDefaultActivationFunction != NULL)
	{
		delete mDefaultActivationFunction;
		mDefaultActivationFunction = NULL;
	}

	viDeleteAll(mActivationFunctions);
	
	mNeurons.clear();
	mBiases.clear();
}

void ViNeuralNetworkFactory::setActivationFunction(ViActivationFunction *activationFunction)
{
	mDefaultActivationFunction = activationFunction;
}

void ViNeuralNetworkFactory::addLayer(int neuronCount)
{
	addLayer(neuronCount, 0, mDefaultActivationFunction->clone());
}

void ViNeuralNetworkFactory::addLayer(int neuronCount, double bias)
{
	addLayer(neuronCount, bias, mDefaultActivationFunction->clone());
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
		if(mActivationFunctions[i] == NULL)
		{
			LOG("No activation function was specified for layer " + QString::number(i + 1) + ". The default activation function will be used.", QtCriticalMsg);
		}
		if(mNeurons[i] <= 0)
		{
			LOG("Layer " + QString::number(i + 1) + " has an invalid amount (" + QString::number(mNeurons[i]) + ") of neurons. At least one neuron must be specified.", QtCriticalMsg);
		}

		if(i == 0)
		{
			if(mBiases[i] != 0)
			{
				LOG("No bias allowed in the input layer. The bias will be removed.", QtCriticalMsg);
			}
			network->add(ViNeuralLayerFactory::createInput(mNeurons[i]));
		}
		else if(i == mNeurons.size() - 1)
		{
			if(mBiases[i] != 0)
			{
				LOG("No bias allowed in the output layer. The bias will be removed.", QtCriticalMsg);
			}
			network->add(ViNeuralLayerFactory::createOutput(mNeurons[i], mActivationFunctions[i]));
		}
		else
		{
			network->add(ViNeuralLayerFactory::createHidden(mNeurons[i], mActivationFunctions[i], mBiases[i]));
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
