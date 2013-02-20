#include <vineuralnetwork.h>

ViNeuralNetwork::ViNeuralNetwork(ViNeuralLayerList layers)
	: ViNeuralUnit(), QRunnable()
{
	mLayers = layers;
}

ViNeuralNetwork::ViNeuralNetwork(const ViNeuralNetwork &other)
	: ViNeuralUnit(other)
{
	mLayers = other.mLayers;
}

ViNeuralNetwork::~ViNeuralNetwork()
{
	qDeleteAll(mLayers);
	mLayers.clear();
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
	if(mLayers.size() >= index)
	{
		LOG("An invalid layer was accessed.", QtCriticalMsg);
		return NULL;
	}
	return mLayers[index];
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
