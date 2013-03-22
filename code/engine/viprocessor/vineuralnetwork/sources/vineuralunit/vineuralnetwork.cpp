#include <vineuralnetwork.h>
#include <vierrorfunctionmanager.h>

#define DEFAULT_LEARNING_RATE 0.1

ViNeuralNetwork::ViNeuralNetwork(ViNeuralLayerList layers)
	: ViNeuralUnit(), QRunnable()
{
	mLayers = layers;
	setLearningRate(DEFAULT_LEARNING_RATE);
	setErrorFunction(NULL);
}

ViNeuralNetwork::ViNeuralNetwork(const ViNeuralNetwork &other)
	: ViNeuralUnit(other)
{
	mLayers = other.mLayers;
	mLearningRate = other.mLearningRate;
	mErrorFunction = other.mErrorFunction->clone();
}

ViNeuralNetwork::~ViNeuralNetwork()
{
	viDeleteAll(mLayers);

	delete mErrorFunction;
	mErrorFunction = NULL;
}

void ViNeuralNetwork::run()
{
	for(int i = 1; i < mLayers.size(); ++i) // Don't run the first/input layer
	{
		mLayers[i]->run();
	}
	//calculate error
	//back propagate
}

bool ViNeuralNetwork::add(ViNeuralLayer *layer)
{
	if(contains(layer))
	{
		
		return false;
	}
	mLayers.append(layer);
	return true;
}

void ViNeuralNetwork::clear()
{
	mLayers.clear();
}

bool ViNeuralNetwork::contains(ViNeuralLayer *layer) const
{
	return mLayers.contains(layer);
}

int ViNeuralNetwork::size() const
{
	return mLayers.size();
}

ViNeuralLayer* ViNeuralNetwork::at(int index) const
{
	if(index >= mLayers.size())
	{
		LOG("An invalid layer was accessed.", QtCriticalMsg);
		return NULL;
	}
	return mLayers[index];
}

ViNeuron* ViNeuralNetwork::neuron(const QString id) const
{
	ViNeuron *neuron = NULL;
	for(int i = 0; i < mLayers.size(); ++i)
	{
		neuron = mLayers[i]->neuron(id);
		if(neuron != NULL)
		{
			break;
		}
	}
	return neuron;
}

void ViNeuralNetwork::setInput(int index, double value)
{
	if(mLayers.isEmpty())
	{
		LOG("The neural network has no input layer.", QtCriticalMsg);
	}
	else
	{
		mLayers[0]->setValue(index, value);
	}
}

void ViNeuralNetwork::setInputs(ViDoubleList values)
{
	if(mLayers.isEmpty())
	{
		LOG("The neural network has no input layer.", QtCriticalMsg);
	}
	else
	{
		int inputSize = inputCount();
		ViNeuralLayer *layer = mLayers[0];
		if(inputSize < values.size())
		{
			LOG("Trying to set more input values than input neurons in the neural network.", QtCriticalMsg);
			for(int i = 0; i < inputSize; ++i)
			{
				layer->setValue(i, values[i]);
			}
		}
	}
}

double ViNeuralNetwork::output(int index)
{
	if(mLayers.isEmpty())
	{
		LOG("The neural network has no output layer.", QtCriticalMsg);
		return 0;
	}
	return mLayers[mLayers.size() - 1]->value(index);
}

ViDoubleList ViNeuralNetwork::outputs()
{
	ViDoubleList outputs;
	int outputSize = outputCount();
	if(outputSize == 0)
	{
		LOG("The neural network has no output layer.", QtCriticalMsg);
	}
	else
	{
		ViNeuralLayer *layer = mLayers[mLayers.size() - 1];
		for(int i = 0; i < outputSize; ++i)
		{
			outputs.append(layer->value(i));
		}
	}
	return outputs;
}

int ViNeuralNetwork::inputCount()
{
	if(mLayers.isEmpty())
	{
		return 0;
	}
	return mLayers[0]->size();
}

int ViNeuralNetwork::outputCount()
{
	if(mLayers.isEmpty())
	{
		return 0;
	}
	return mLayers[mLayers.size() - 1]->size();
}

int ViNeuralNetwork::neuronCount()
{
	int count = 0;
	for(int i = 0; i < mLayers.size(); ++i)
	{
		count += mLayers[i]->size();
	}
	return count;
}

void ViNeuralNetwork::setLearningRate(qreal learningRate)
{
	mLearningRate = learningRate;
}

qreal ViNeuralNetwork::learningRate()
{
	return mLearningRate;
}

void ViNeuralNetwork::setErrorFunction(ViErrorFunction *function)
{
	mErrorFunction = function;
}

ViErrorFunction* ViNeuralNetwork::errorFunction()
{
	return mErrorFunction;
}

ViNeuralLayer* ViNeuralNetwork::operator [] (const int index) const
{
	return at(index);
}

bool ViNeuralNetwork::operator == (const ViNeuralNetwork &other) const
{
	if(mLayers.size() != other.mLayers.size())
	{
		return false;
	}

	for(int i = 0; i < mLayers.size(); ++i)
	{
		if(!other.contains(mLayers[i]))
		{
			return false;
		}
	}

	return true;
}

ViElement ViNeuralNetwork::exportData()
{
	ViElement element("NeuralNetwork");

	element.addChild("LearningRate", learningRate());

	if(errorFunction() == NULL)
	{
		element.addChild(errorFunction()->exportData());
	}
	else
	{
		LOG("There is no error function to export.", QtCriticalMsg);
	}
	
	ViElement neuronLayers("NeuronLayers");
	neuronLayers.addAttribute("count", mLayers.size());
	for(int i = 0; i < mLayers.size(); ++i)
	{
		neuronLayers.addChild(mLayers[i]->exportData());	
	}
	element.addChild(neuronLayers);

	ViElement synapseLayers("SynapseLayers");
	synapseLayers.addAttribute("count", mLayers.size() - 1);
	for(int i = 1; i < mLayers.size(); ++i)
	{
		ViElement synapseLayer("SynapseLayer");
		synapseLayer.addAttribute("count", mLayers[i]->inputSize());
		for(int j = 0; j < mLayers[i]->size(); ++j)
		{
			for(int k = 0; k < mLayers[i]->at(j)->inputSize(); ++k)
			{
				synapseLayer.addChild(mLayers[i]->at(j)->inputAt(k)->exportData());
			}
		}
		synapseLayers.addChild(synapseLayer);
	}
	element.addChild(synapseLayers);

	return element;
}

bool ViNeuralNetwork::importData(ViElement element)
{
	if(element.name() != "NeuralNetwork")
	{
		return false;
	}
	setLearningRate(element.child("LearningRate").toReal());
	setErrorFunction(ViErrorFunctionManager::create(element));
	if(errorFunction() == NULL)
	{
		return false;
	}
	return true;
}
