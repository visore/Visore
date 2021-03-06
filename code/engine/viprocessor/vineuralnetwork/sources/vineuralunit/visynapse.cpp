#include <visynapse.h>
#include <vineuron.h>

ViSynapse::ViSynapse()
	: ViNeuralUnit()
{
	setInput(NULL);
	setOutput(NULL);
	mWeight = 1;
}

ViSynapse::ViSynapse(ViNeuron *input, ViNeuron *output, double weight)
	: ViNeuralUnit()
{
	setInput(input);
	setOutput(output);
	mWeight = weight;
}

ViSynapse::ViSynapse(const ViSynapse &other)
	: ViNeuralUnit(other)
{
	setInput(other.mInput);
	setOutput(other.mOutput);
	mWeight = other.mWeight;
}

ViNeuron* ViSynapse::input() const
{
	return mInput;
}

ViNeuron* ViSynapse::output() const
{
	return mOutput;
}

double ViSynapse::weight() const
{
	return mWeight;
}

double ViSynapse::value() const
{
	return mWeight * mInput->value();
}
		
void ViSynapse::setInput(ViNeuron *neuron)
{
	mInput = neuron;
	if(mInput != NULL)
	{
		mInput->addOutput(this);
	}
}

void ViSynapse::setOutput(ViNeuron *neuron)
{
	mOutput = neuron;
	if(mOutput != NULL)
	{
		mOutput->addInput(this);
	}
}

void ViSynapse::setWeight(double weight)
{
	mWeight = weight;
}

bool ViSynapse::operator == (const ViSynapse &other) const
{
	return mInput == other.mInput && mOutput == other.mOutput && mWeight == other.mWeight;
}

ViElement ViSynapse::exportData()
{
	ViElement element("synapse");
	element.addChild("inputneuron", input()->id());
	element.addChild("outputneuron", output()->id());
	element.addChild("weight", weight());
	return element;
}

bool ViSynapse::importData(ViElement element)
{
	if(element.name() != "synapse")
	{
		return false;
	}
	ViElement weight = element.child("weight");
	if(weight.isNull())
	{
		LOG("The weight for the synapse could not be retrieved. Setting the default weight of " + QString::number(DEFAULT_SYNAPSE_WEIGHT) + ".");
		setWeight(DEFAULT_SYNAPSE_WEIGHT);
	}
	else
	{
		setWeight(weight.toReal());
	}
	return true;
}
