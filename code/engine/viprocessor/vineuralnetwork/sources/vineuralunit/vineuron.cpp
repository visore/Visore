#include <vineuron.h>
#include <vineurallayer.h>
#include <viactivationfunctionmanager.h>

ViNeuron::ViNeuron()
	: ViNeuralUnit(), QRunnable()
{
	mActivationFunction = NULL;
	mValue = 0;
	mError = 0;
	mType = ViNeuron::UnknownNeuron;
}

ViNeuron::ViNeuron(ViNeuron::Type type, ViActivationFunction *activationFunction)
	: ViNeuralUnit(), QRunnable()
{
	mActivationFunction = activationFunction;
	mValue = 0;
	mError = 0;
	mType = type;
}

ViNeuron::ViNeuron(const ViNeuron &other)
	: ViNeuralUnit(other)
{
	if(other.mActivationFunction != NULL)
	{
		mActivationFunction = other.mActivationFunction->clone();
	}
	mValue = other.mValue;
	mError = other.mError;
	mType = other.mType;
}

ViNeuron::~ViNeuron()
{
	if(mActivationFunction != NULL)
	{
		delete mActivationFunction;
		mActivationFunction = NULL;
	}

	viDeleteAll(mInputs);
	viDeleteAll(mOutputs);
}

void ViNeuron::run()
{
	double sum = 0;
	for(int i = 0; i < mInputs.size(); ++i)
	{
		sum += mInputs[i]->value();
	}
	mValue = mActivationFunction->calculate(sum);
}

ViNeuron::Type ViNeuron::type()
{
	return mType;
}
		
void ViNeuron::setType(ViNeuron::Type type)
{
	mType = type;
}

double ViNeuron::value()
{
	return mValue;
}

void ViNeuron::setValue(double value)
{
	mValue = value;
}

double ViNeuron::error()
{
	return mError;
}

void ViNeuron::setError(double error)
{
	mError = error;
}

ViActivationFunction* ViNeuron::activationFunction() const
{
	return mActivationFunction;
}

void ViNeuron::setActivationFunction(ViActivationFunction *activationFunction)
{
	if(mActivationFunction != NULL)
	{
		delete mActivationFunction;
	}
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
	if(mValue != other.mValue)
	{
		return false;
	}
	if(mType == other.mType)
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

ViElement ViNeuron::exportData()
{
	ViElement element("neuron");
	element.addChild("id", id());

	if(type() == ViNeuron::UnknownNeuron)
	{
		LOG("An unknown neuron (id: " + id() + ") was detected.", QtCriticalMsg);
	}
	element.addChild("type", typeToString(type()));

	if(activationFunction() != NULL)
	{
		element.addChild(activationFunction()->exportData());
	}

	if(type() == ViNeuron::BiasNeuron)
	{
		element.addChild("value", value());
	}

	return element;
}

bool ViNeuron::importData(ViElement element)
{
	if(element.name() != "neuron")
	{
		return false;
	}

	bool success = true;

	ViElement id = element.child("id");
	if(id.isNull())
	{
		success = false;
	}
	else
	{
		setId(id.toString());
	}

	ViElement theType = element.child("type");
	if(theType.isNull())
	{
		success = false;
	}
	else
	{
		setType(stringToType(theType.toString()));
	}

	if(type() == ViNeuron::HiddenNeuron || type() == ViNeuron::OutputNeuron)
	{
		if(id.isNull())
		{
			success = false;
			setActivationFunction(NULL);
		}
		else
		{
			setActivationFunction(ViActivationFunctionManager::create(element));
		}
		if(mActivationFunction == NULL)
		{
			setActivationFunction(ViActivationFunctionManager::createDefault());
			LOG("Unable to create the required activation function from the import data. The default activation function (" + mActivationFunction->name() + ") will be used.", QtCriticalMsg);
		}
		else
		{
			if(!mActivationFunction->importData(element))
			{
				success = false;
			}
		}
	}
	else if(type() == ViNeuron::BiasNeuron)
	{
		ViElement value = element.child("value");
		if(value.isNull())
		{
			success = false;
			setActivationFunction(NULL);
		}
		else
		{
			setValue(value.toReal());
		}
	}

	return success;
}

QString ViNeuron::typeToString(ViNeuron::Type type)
{
	if(type == ViNeuron::InputNeuron)
	{
		return "Input";
	}
	else if(type == ViNeuron::OutputNeuron)
	{
		return "Output";
	}
	else if(type == ViNeuron::HiddenNeuron)
	{
		return "Hidden";
	}
	else if(type == ViNeuron::BiasNeuron)
	{
		return "Bias";
	}
	return "Unknown";
}

ViNeuron::Type ViNeuron::stringToType(QString type)
{
	type = type.trimmed().toLower();
	if(type == "input")
	{
		return ViNeuron::InputNeuron;
	}
	else if(type == "output")
	{
		return ViNeuron::OutputNeuron;
	}
	else if(type == "hidden")
	{
		return ViNeuron::HiddenNeuron;
	}
	else if(type == "bias")
	{
		return ViNeuron::BiasNeuron;
	}
	return ViNeuron::UnknownNeuron;
}
