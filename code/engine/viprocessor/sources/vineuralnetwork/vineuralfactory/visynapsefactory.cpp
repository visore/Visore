#include <visynapsefactory.h>
#include <vineuralnetwork.h>

ViSynapse* ViSynapseFactory::create(ViElement element, ViNeuralNetwork *network, bool &error)
{
	ViSynapse *synapse = new ViSynapse();
	if(synapse->importData(element))
	{
		bool success = true;
		ViNeuron *input = network->neuron(element.child("InputNeuron").toString());
		if(input == NULL)
		{
			success = false;
			STATICLOG("The input neuron (" + element.child("InputNeuron").toString() + ") is not in the neural network.", QtCriticalMsg, "ViSynapseFactory");
		}
		else
		{
			ViNeuron *output = network->neuron(element.child("OutputNeuron").toString());
			if(output == NULL)
			{
				success = false;
				STATICLOG("The output neuron (" + element.child("OutputNeuron").toString() + ") is not in the neural network.", QtCriticalMsg, "ViSynapseFactory");
			}
			else
			{
				synapse->setInput(input);
				synapse->setOutput(output);
			}
		}
		if(!success)
		{
			error = true;
		}
	}
	else
	{
		error = true;
		delete synapse;
		synapse = NULL;
	}	
	return synapse;
}

QList<ViSynapse*> ViSynapseFactory::create(ViElementList elements, ViNeuralNetwork *network, bool &error)
{
	QList<ViSynapse*> synapses;
	for(int i = 0; i < elements.size(); ++i)
	{
		synapses.append(create(elements[i], network, error));
	}
	return synapses;
}

ViSynapse* ViSynapseFactory::create(ViNeuron *input, ViNeuron *output, double weight)
{
	ViSynapse *synapse = new ViSynapse(input, output, weight);
	return synapse;
}
