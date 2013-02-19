#include <vineuralnetwork.h>

ViNeuralNetwork::ViNeuralNetwork(ViNeuralLayerList layers)
	: ViNeuralUnit()
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
		return NULL;
	}
	return mLayers[index];
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
