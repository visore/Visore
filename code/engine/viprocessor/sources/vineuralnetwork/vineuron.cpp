#include <vineuron.h>

ViNeuron::ViNeuron(ViActivationFunction *activationFunction)
	: ViNeuralUnit()
{
	mActivationFunction = activationFunction;
}

ViNeuron::ViNeuron(const ViNeuron &other)
	: ViNeuralUnit(other)
{
	mActivationFunction = other.mActivationFunction;
}

ViNeuron::~ViNeuron()
{
	qDeleteAll(mInputs);
	mInputs.clear();

	qDeleteAll(mOutputs);
	mOutputs.clear();
}

ViActivationFunction* ViNeuron::activationFunction() const
{
	return mActivationFunction;
}

void ViNeuron::setActivationFunction(ViActivationFunction *activationFunction)
{
	mActivationFunction = activationFunction;
}

bool ViNeuron::add(Vi::Direction direction, ViSynapse *synapse)
{
	if(direction == Vi::Input)
	{
		return addInput(synapse);
	}
	else if(direction == Vi::Output)
	{
		return addOutput(synapse);
	}
}

bool ViNeuron::addInput(ViSynapse *synapse)
{
	if(containsInput(synapse))
	{
		return false;
	}
	mInputs.append(synapse);
	return true;
}

bool ViNeuron::addOutput(ViSynapse *synapse)
{
	if(containsOutput(synapse))
	{
		return false;
	}
	mOutputs.append(synapse);
	return true;
}

bool ViNeuron::contains(ViSynapse *synapse) const
{
	return containsInput(synapse) || containsOutput(synapse);
}

bool ViNeuron::contains(Vi::Direction direction, ViSynapse *synapse) const
{
	if(direction == Vi::Input)
	{
		return containsInput(synapse);
	}
	else if(direction == Vi::Output)
	{
		return containsOutput(synapse);
	}
}

bool ViNeuron::containsInput(ViSynapse *synapse) const
{
	for(int i = 0; i < mInputs.size(); ++i)
	{
		if(*mInputs[i] == *synapse)
		{
			return true;
		}
	}
	return false;
}

bool ViNeuron::containsOutput(ViSynapse *synapse) const
{
	for(int i = 0; i < mOutputs.size(); ++i)
	{
		if(*mOutputs[i] == *synapse)
		{
			return true;
		}
	}
	return false;
}

int ViNeuron::inputSize() const
{
	return mInputs.size();
}

int ViNeuron::outputSize() const
{
	return mOutputs.size();
}

ViSynapse* ViNeuron::inputAt(int index) const
{
	return mInputs[index];
}

ViSynapse* ViNeuron::outputAt(int index) const
{
	return mOutputs[index];
}

bool ViNeuron::operator == (const ViNeuron &other) const
{
	if(mActivationFunction != other.mActivationFunction)
	{
		return false;
	}
	if(mInputs.size() != other.mInputs.size() || mOutputs.size() != other.mOutputs.size())
	{
		return false;
	}

	for(int i = 0; i < mInputs.size(); ++i)
	{
		if(!other.containsInput(mInputs[i]))
		{
			return false;
		}
	}

	for(int i = 0; i < mOutputs.size(); ++i)
	{
		if(!other.containsOutput(mOutputs[i]))
		{
			return false;
		}
	}

	return true;
}
