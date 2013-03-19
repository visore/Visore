#include <viweightinitializer.h>

ViWeightInitializer::ViWeightInitializer()
{
	mCurrentSynapse = -1;
	mCurrentInputNeuron = -1;
	mCurrentOutputNeuron = -1;
	mCurrentInputLayer = -1;
	mCurrentOutputLayer = -1;
	mCurrentInputLayerNeuron = -1;
	mCurrentOutputLayerNeuron = -1;
	mCurrentInputActivationFunction = NULL;
	mCurrentOutputActivationFunction = NULL;
}

ViWeightInitializer::ViWeightInitializer(const ViWeightInitializer &other)
{
	mCurrentSynapse = other.mCurrentSynapse;
	mCurrentInputNeuron = other.mCurrentInputNeuron;
	mCurrentOutputNeuron = other.mCurrentOutputNeuron;
	mCurrentInputLayer = other.mCurrentInputLayer;
	mCurrentOutputLayer = other.mCurrentOutputLayer;
	mCurrentInputLayerNeuron = other.mCurrentInputLayerNeuron;
	mCurrentOutputLayerNeuron = other.mCurrentOutputLayerNeuron;
	mCurrentInputActivationFunction = other.mCurrentInputActivationFunction;
	mCurrentOutputActivationFunction = other.mCurrentOutputActivationFunction;
}

ViWeightInitializer::~ViWeightInitializer()
{
}

const int& ViWeightInitializer::currentSynapse() const
{
	return mCurrentSynapse;
}

const int& ViWeightInitializer::currentInputNeuron() const
{
	return mCurrentInputNeuron;
}

const int& ViWeightInitializer::currentOutputNeuron() const
{
	return mCurrentOutputNeuron;
}

const int& ViWeightInitializer::currentInputLayer() const
{
	return mCurrentInputLayer;
}

const int& ViWeightInitializer::currentOutputLayer() const
{
	return mCurrentOutputLayer;
}

const int& ViWeightInitializer::currentInputLayerNeuron() const
{
	return mCurrentInputLayerNeuron;
}

const int& ViWeightInitializer::currentOutputLayerNeuron() const
{
	return mCurrentOutputLayerNeuron;
}

const ViActivationFunction* ViWeightInitializer::currentInputActivationFunction() const
{
	return mCurrentInputActivationFunction;
}

const ViActivationFunction* ViWeightInitializer::currentOutputActivationFunction() const
{
	return mCurrentOutputActivationFunction;
}

void ViWeightInitializer::initialize(ViNeuralNetwork *network)
{
	ViNeuralLayer *inputLayer, *outputLayer;
	for(mCurrentSynapse = 0,
		mCurrentInputNeuron = 0,
		mCurrentOutputNeuron = 0,
		mCurrentInputLayer = 0,
		mCurrentOutputLayer = mCurrentInputLayer + 1;
			mCurrentInputLayer < network->size() - 1;
				++mCurrentInputLayer,
				++mCurrentOutputLayer)
	{
		inputLayer = network->at(mCurrentInputLayer);
		outputLayer = network->at(mCurrentOutputLayer);
		
		//For normal synapses
		for(mCurrentInputLayerNeuron = 0;
				mCurrentInputLayerNeuron < inputLayer->size();
					++mCurrentInputLayerNeuron,
					++mCurrentInputNeuron)
		{
			initialize(network, inputLayer);
		}

		//For bias synapses
		if(outputLayer->hasBias())
		{
			++mCurrentInputLayerNeuron;
			++mCurrentInputNeuron;
			initialize(network, outputLayer, outputLayer->bias());
		}
	}
}

void ViWeightInitializer::initialize(ViNeuralNetwork *network, ViNeuralLayer *inputLayer, ViNeuron *neuron)
{
	ViNeuron *inputNeuron, *outputNeuron;
	ViSynapse *synapse;
	if(neuron == NULL)
	{
		inputNeuron = inputLayer->at(mCurrentInputLayerNeuron);
	}
	else
	{
		inputNeuron = neuron;
	}
	mCurrentInputActivationFunction = inputNeuron->activationFunction();
	for(int i = 0;
			i < inputNeuron->outputSize();
				++i,
				++mCurrentSynapse)
	{
		synapse = inputNeuron->outputAt(i);
		outputNeuron = synapse->output();
		mCurrentOutputActivationFunction = outputNeuron->activationFunction();
		mCurrentOutputNeuron = networkPosition(network, outputNeuron);
		mCurrentOutputLayerNeuron = layerPosition(network, mCurrentOutputLayer, outputNeuron);
		initialize(synapse);
	}
}

ViElement ViWeightInitializer::exportData()
{
	ViElement element("WeightInitializer");
	element.addChild("Name", name());
	return element;
}

bool ViWeightInitializer::importData(ViElement element)
{
	if(element.name() == "WeightInitializer" && element.child("Name").toString() == name())
	{
		return true;
	}
	return false;
}

int ViWeightInitializer::layerPosition(ViNeuralNetwork *network, int layerPosition, ViNeuron *neuron)
{
	ViNeuralLayer *layer = network->at(layerPosition);
	for(int i = 0; i < layer->size(); ++i)
	{
		if(layer->at(i) == neuron)
		{
			return i;
		}
	}
	LOG("The neuron was not found in the current layer. Weights may not be initialized as expected.", QtCriticalMsg);
	return -1;
}

int ViWeightInitializer::networkPosition(ViNeuralNetwork *network, ViNeuron *neuron)
{
	ViNeuralLayer *layer;
	int position = 0;
	for(int i = 0; i < network->size(); ++i)
	{
		layer = network->at(i);
		for(int j = 0; j < layer->size(); ++j, ++position)
		{
			if(layer->at(j) == neuron)
			{
				return position;
			}
		}
	}
	LOG("The neuron was not found in the network. Weights may not be initialized as expected.", QtCriticalMsg);
	return -1;
}
