#include <vineuralnetwork.h>
#include <visynapsefactory.h>

ViNeuralNetwork::ViNeuralNetwork(ViNeuralLayerList layers)
	: ViNeuralUnit(), QRunnable()
{
	mLayers = layers;
}

ViNeuralNetwork::ViNeuralNetwork(const ViNeuralNetwork &other)
	: ViNeuralUnit(other)
{
	for(int i = 0; i < other.mLayers.size(); ++i)
	{
		add(new ViNeuralLayer(*other.mLayers[i]));
	}

	ViNeuralLayer *currentLayer, *nextLayer;
	ViNeuron *currentNeuron;
	ViSynapse *currentSynapse;
	int position1, position2;
	for(int i = 0; i < other.mLayers.size() - 1; ++i)
	{
		currentLayer = other.mLayers[i];
		nextLayer = other.mLayers[i + 1];
		int neurons = currentLayer->size() + currentLayer->hasBias();
		for(int j = 0; j < neurons; ++j)
		{
			currentNeuron = currentLayer->at(j);
			for(int k = 0; k < currentNeuron->outputSize(); ++k)
			{
				currentSynapse = currentNeuron->outputAt(k);
				position1 = currentLayer->position(currentSynapse->input());
				position2 = nextLayer->position(currentSynapse->output());
				ViSynapseFactory::create(at(i)->at(position1), at(i + 1)->at(position2), currentSynapse->weight());
			}
		}
	}

	mHistory = other.mHistory;
}

ViNeuralNetwork::~ViNeuralNetwork()
{
	clear();
	viDeleteAll(mLayers);
}

void ViNeuralNetwork::run()
{
	//Set the histroy values
	if(history() > 0)
	{
		for(int i = 0; i < outputCount(); ++i)
		{
			mHistory.dequeue();
			mHistory.enqueue(output(i));
		}
		int offset = inputCount(false);
		for(int i = 0; i < mHistory.size(); ++i)
		{
			setInput(i + offset, mHistory[i]);
		}
	}

	//Don't run the first/input layer
	for(int i = 1; i < mLayers.size(); ++i)
	{
		mLayers[i]->run();
	}
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
	mHistory.clear();
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

void ViNeuralNetwork::setHistory(int neuronCount)
{
	mHistory.clear();
	for(int i = 0; i < neuronCount; ++i)
	{
		mHistory.enqueue(0);
	}
}

int ViNeuralNetwork::history()
{
	return mHistory.size();
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
		}
		for(int i = 0; i < inputSize; ++i)
		{
			layer->setValue(i, values[i]);
		}
	}
}

double ViNeuralNetwork::input(int index)
{
	if(mLayers.isEmpty())
	{
		LOG("The neural network has no input layer.", QtCriticalMsg);
		return 0;
	}
	return mLayers[0]->value(index);
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

int ViNeuralNetwork::inputCount(bool includeHistory)
{
	if(mLayers.isEmpty())
	{
		return 0;
	}
	else if(includeHistory)
	{
		return mLayers[0]->size();
	}
	else
	{
		return mLayers[0]->size() - mHistory.size();
	}
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

ViNeuralNetwork* ViNeuralNetwork::clone()
{
	return new ViNeuralNetwork(*this);
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
	return true;
}
