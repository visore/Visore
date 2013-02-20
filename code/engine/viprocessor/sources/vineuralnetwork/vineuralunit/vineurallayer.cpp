#include <vineurallayer.h>
#include <QThreadPool>

ViNeuralLayer::ViNeuralLayer(ViNeuronList neurons)
	: ViNeuralUnit(), QRunnable()
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

void ViNeuralLayer::run()
{
	QThreadPool *pool = QThreadPool::globalInstance();
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		pool->start(mNeurons[i]);
	}
	pool->waitForDone();
}

bool ViNeuralLayer::add(ViNeuron *neuron)
{
	if(contains(neuron))
	{
		LOG("The neuron was already added to the layer.", QtCriticalMsg);
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
	if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed.", QtCriticalMsg);
		return NULL;
	}
	return mNeurons[index];
}

double ViNeuralLayer::value(int index)
{
	if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed.", QtCriticalMsg);
		return 0;
	}
	return mNeurons[index]->value();
}

void ViNeuralLayer::setValue(int index, int value)
{
	if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed.", QtCriticalMsg);
	}
	else
	{
		mNeurons[index]->setValue(value);
	}
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
