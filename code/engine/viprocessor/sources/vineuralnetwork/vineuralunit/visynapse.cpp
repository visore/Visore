#include <visynapse.h>
#include <vineuron.h>

ViSynapse::ViSynapse()
	: ViNeuralUnit()
{
	mInput = NULL;
	mOutput = NULL;
	mWeight = 1;
}

ViSynapse::ViSynapse(ViNeuron *input, ViNeuron *output, double weight)
	: ViNeuralUnit()
{
	mInput = input;
	mOutput = output;
	mWeight = weight;
}

ViSynapse::ViSynapse(const ViSynapse &other)
	: ViNeuralUnit(other)
{
	mInput = other.mInput;
	mOutput = other.mOutput;
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
}

void ViSynapse::setOutput(ViNeuron *neuron)
{
	mOutput = neuron;
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
	ViElement element("Synapse");
	element.addChild("InputNeuron", input()->id());
	element.addChild("OutputNeuron", output()->id());
	element.addChild("Weight", weight());
	return element;
}

bool ViSynapse::importData(ViElement element)
{
	if(element.name() != "Synapse")
	{
		return false;
	}
	ViElement weight = element.child("Weight");
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
