#include <vineuralnetworkfactory.h>
#include <vineurallayerfactory.h>
#include <visynapsefactory.h>
#include <vistaticneuronfactory.h>
#include <viactivationfunctionmanager.h>

ViNeuralNetworkFactory::ViNeuralNetworkFactory()
{
	mGlobalActivationFunction = NULL;
	mWeightInitializer = NULL;
}

ViNeuralNetworkFactory::ViNeuralNetworkFactory(const ViNeuralNetworkFactory &other)
{
	mGlobalActivationFunction = other.mGlobalActivationFunction;
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
	if(mGlobalActivationFunction != NULL)
	{
		delete mGlobalActivationFunction;
		mGlobalActivationFunction = NULL;
	}

	if(mWeightInitializer != NULL)
	{
		delete mWeightInitializer;
		mWeightInitializer = NULL;
	}

	viDeleteAll(mActivationFunctions);
	
	mNeurons.clear();
	mBiases.clear();
}

void ViNeuralNetworkFactory::setActivationFunction(ViActivationFunction *activationFunction)
{
	if(mGlobalActivationFunction != NULL)
	{
		delete mGlobalActivationFunction;
	}
	mGlobalActivationFunction = activationFunction;
}

void ViNeuralNetworkFactory::setWeightInitializer(ViWeightInitializer *weightInitializer)
{
	if(mWeightInitializer != NULL)
	{
		delete mWeightInitializer;
	}
	mWeightInitializer = weightInitializer;
}

void ViNeuralNetworkFactory::addLayer(int neuronCount)
{
	addLayer(neuronCount, 0, mGlobalActivationFunction->clone());
}

void ViNeuralNetworkFactory::addLayer(int neuronCount, double bias)
{
	addLayer(neuronCount, bias, mGlobalActivationFunction->clone());
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
		ViActivationFunction *activationFunction = mActivationFunctions[i];
		bool deleteFunction = false;
		if(activationFunction == NULL)
		{
			deleteFunction = true;
			activationFunction = ViActivationFunctionManager::createDefault();
			LOG("No activation function was specified for layer " + QString::number(i + 1) + ". The default activation function (" + activationFunction->name() + ") will be used.", QtCriticalMsg);
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
			network->add(ViNeuralLayerFactory::createOutput(mNeurons[i], activationFunction));
		}
		else
		{
			network->add(ViNeuralLayerFactory::createHidden(mNeurons[i], activationFunction, mBiases[i]));
		}
		if(deleteFunction)
		{
			delete activationFunction;
		}
	}

	for(int i = 0; i < network->size() - 1; ++i)
	{
		for(int j = 0; j < network->at(i)->size(); ++j)
		{
			for(int k = 0; k < network->at(i + 1)->size(); ++k)
			{
				ViSynapseFactory::create(network->at(i)->at(j), network->at(i + 1)->at(k));
			}
		}
	}

	if(mWeightInitializer == NULL)
	{
		setWeightInitializer(ViWeightInitializer::defaultInitializer());
	}
	mWeightInitializer->initialize(network);

	return network;
}

ViNeuralNetwork* ViNeuralNetworkFactory::create(ViElement element)
{
	ViNeuralNetwork *network = new ViNeuralNetwork();
	bool success = true;
	if(network->importData(element))
	{
		bool error = false;

		//Create neuron layers
		ViElement neuronLayerElement = element.child("NeuronLayers");
		if(neuronLayerElement.isNull())
		{
			success = false;
		}
		ViElementList neuronLayerElements = neuronLayerElement.children("NeuronLayer");
		if(neuronLayerElements.size() < 2)
		{
			LOG("The neural network doesn't have enough layers.", QtCriticalMsg);
			success = false;
		}
		for(int i = 0; i < neuronLayerElements.size(); ++i)
		{
			error = false;
			ViNeuralLayer *layer = ViNeuralLayerFactory::create(neuronLayerElements[i], error);
			if(error || layer == NULL)
			{
				success = false;
			}
			else
			{
				network->add(layer);
			}
		}
		
		//Create synapse layers
		ViElement synapseLayerElement = element.child("SynapseLayers");
		if(synapseLayerElement.isNull())
		{
			success = false;
		}
		ViElementList synapseLayerElements = synapseLayerElement.children("SynapseLayer");
		if(neuronLayerElements.size() == 0)
		{
			LOG("The neural network doesn't have any synapses.", QtCriticalMsg);
			success = false;
		}
		for(int i = 0; i < synapseLayerElements.size(); ++i)
		{
			error = false;
			ViSynapseFactory::create(synapseLayerElements[i].children("Synapse"), network, error);
			if(error)
			{
				success = false;
			}
		}
	}
	else
	{
		success = false;
	}

	if(!success)
	{
		LOG("The neural network could not be fully imported and may not behave as expected.", QtCriticalMsg);
	}

	return network;
}
