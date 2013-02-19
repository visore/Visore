#include <vineurallayer.h>

ViNeuralLayer::ViNeuralLayer(ViNeuronList neurons)
	: ViNeuralUnit()
{
	mNeurons = neurons;
}

ViNeuralLayer::ViNeuralLayer(const ViNeuralLayer &other)
	: ViNeuralUnit(other)
{
	mNeurons = other.mNeurons;
}

ViNeuralLayer::~ViNeuralLayer()
{
	qDeleteAll(mNeurons);
	mNeurons.clear();
}

bool ViNeuralLayer::add(ViNeuron *neuron)
{
	if(contains(neuron))
	{
		return false;
	}
	mNeurons.append(neuron);
	return true;
}

void ViNeuralLayer::clear()
{
	mNeurons.clear();
}

bool ViNeuralLayer::contains(ViNeuron *neuron) const
{
	return mNeurons.contains(neuron);
}

int ViNeuralLayer::size() const
{
	return mNeurons.size();
}

ViNeuron* ViNeuralLayer::at(int index) const
{
	if(mNeurons.size() >= index)
	{
		return NULL;
	}
	return mNeurons[index];
}

ViNeuron* ViNeuralLayer::operator [] (const int index) const
{
	return at(index);
}

bool ViNeuralLayer::operator == (const ViNeuralLayer &other) const
{
	if(mNeurons.size() != other.mNeurons.size())
	{
		return false;
	}

	for(int i = 0; i < mNeurons.size(); ++i)
	{
		if(!other.contains(mNeurons[i]))
		{
			return false;
		}
	}

	return true;
}
