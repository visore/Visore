#include <vineurallayer.h>
#include <QThreadPool>

ViNeuralLayer::ViNeuralLayer(ViNeuronList neurons)
	: ViNeuralUnit(), QRunnable()
{
	mNeurons = neurons;
	mBias = NULL;
}

ViNeuralLayer::ViNeuralLayer(const ViNeuralLayer &other)
	: ViNeuralUnit(other)
{
	for(int i = 0; i < other.mNeurons.size(); ++i)
	{
		add(new ViNeuron(*other.mNeurons[i]));
	}
	if(other.mBias != NULL)
	{
		mBias = new ViNeuron(*other.mBias);
	}
}

ViNeuralLayer::~ViNeuralLayer()
{
	viDeleteAll(mNeurons);
	if(mBias != NULL)
	{
		delete mBias;
		mBias = NULL;
	}
}

void ViNeuralLayer::run()
{
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		mNeurons[i]->run();
	}
}

bool ViNeuralLayer::add(ViNeuron *neuron)
{
	if(contains(neuron))
	{
		LOG("The neuron was already added to the layer.", QtCriticalMsg);
		return false;
	}
	if(neuron->type() == ViNeuron::BiasNeuron)
	{
		LOG("The added neuron is a bias. The bias will be set, but not added to the neuron list.", QtCriticalMsg);
		setBias(neuron);
	}
	else
	{
		mNeurons.append(neuron);
	}
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

int ViNeuralLayer::inputSize()
{
	int count = 0;
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		count += mNeurons[i]->inputSize();
	}
	return count;
}

int ViNeuralLayer::outputSize()
{
	int count = 0;
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		count += mNeurons[i]->outputSize();
	}
	return count;
}

ViNeuron* ViNeuralLayer::at(int index) const
{
	if(hasBias() && index == mNeurons.size())
	{
		return mBias;
	}
	else if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed (index: " + QString::number(index) + ", size: " + QString::number(mNeurons.size()) + ").", QtCriticalMsg);
		return NULL;
	}
	return mNeurons[index];
}

ViNeuron* ViNeuralLayer::neuron(const QString id) const
{
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		if(mNeurons[i]->id() == id)
		{
			return mNeurons[i];
		}
	}
	if(hasBias() && mBias->id() == id)
	{
		return mBias;
	}
	return NULL;
}

ViNeuron* ViNeuralLayer::bias()
{
	return mBias;
}

bool ViNeuralLayer::setBias(ViNeuron *bias)
{
	if(bias->type() != ViNeuron::BiasNeuron)
	{
		LOG("The neuron is not a bias.");
		return false;
	}
	if(mBias != NULL)
	{
		delete mBias;
	}
	mBias = bias;
	return true;
}

bool ViNeuralLayer::hasBias() const
{
	return mBias != NULL;
}

double ViNeuralLayer::value(int index)
{
	if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed (index: " + QString::number(index) + ", size: " + QString::number(mNeurons.size()) + ").", QtCriticalMsg);
		return 0;
	}
	return mNeurons[index]->value();
}

void ViNeuralLayer::setValue(int index, int value)
{
	if(index >= mNeurons.size())
	{
		LOG("An invalid neuron was accessed (index: " + QString::number(index) + ", size: " + QString::number(mNeurons.size()) + ").", QtCriticalMsg);
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

ViNeuron* ViNeuralLayer::operator [] (const QString id) const
{
	return neuron(id);
}

bool ViNeuralLayer::operator == (const ViNeuralLayer &other) const
{
	if(mNeurons.size() != other.mNeurons.size())
	{
		return false;
	}
	if(*mBias != *other.mBias)
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

ViElement ViNeuralLayer::exportData()
{
	ViElement element("NeuronLayer");
	element.addAttribute("count", mNeurons.size());
	for(int i = 0; i < mNeurons.size(); ++i)
	{
		element.addChild(mNeurons[i]->exportData());
	}
	if(mBias != NULL)
	{
		element.addChild(mBias->exportData());
	}
	return element;
}

bool ViNeuralLayer::importData(ViElement element)
{
	if(element.name() != "NeuronLayer")
	{
		return false;
	}
	return true;
}
