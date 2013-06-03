#include <vineurallayerfactory.h>
#include <vineuronfactory.h>
#include <vistaticneuronfactory.h>
#include <visynapsefactory.h>

ViNeuralLayer* ViNeuralLayerFactory::create(ViElement element, bool &error)
{
	ViNeuralLayer *layer = new ViNeuralLayer();
	if(layer->importData(element))
	{
		bool success = true;
		int neuronCount = element.attribute("count").toInt();
		ViElementList neuronElements = element.children("Neuron");
		
		//Accomodate the Bias
		for(int i = neuronElements.size() - 1; i >= 0; --i)
		{
			if(ViNeuron::stringToType(neuronElements[i].child("Type").toString()) == ViNeuron::BiasNeuron)
			{
				++neuronCount;
				break;
			}
		}

		if(neuronCount != neuronElements.size())
		{
			success = false;
			STATICLOG("Neuron count mismatch detected. " + QString::number(neuronCount) + " expected, but " + QString::number(neuronElements.size()) + " found." + element.child("Bias").name() , QtCriticalMsg, "ViNeuralLayerFactory");
		}

		for(int i = 0; i < neuronElements.size(); ++i)
		{
			ViNeuron::Type type = ViNeuron::stringToType(neuronElements[i].child("Type").toString());
			if(type == ViNeuron::UnknownNeuron)
			{
				success = false;
				STATICLOG("An unsupported neuron was detected and removed from the layer.", QtCriticalMsg, "ViNeuralLayerFactory");
			}
			else
			{
				ViNeuron *neuron = NULL;
				if(type == ViNeuron::InputNeuron || type == ViNeuron::BiasNeuron)
				{
					neuron = ViStaticNeuronFactory::create(type);
				}
				else if(type == ViNeuron::OutputNeuron || type == ViNeuron::HiddenNeuron)
				{
					neuron = ViNeuronFactory::create(type);
				}
				neuron->importData(neuronElements[i]);
				if(type == ViNeuron::BiasNeuron)
				{
					layer->setBias(neuron);
				}
				else
				{
					layer->add(neuron);
				}
			}
		}

		ViNeuron *bias = layer->bias();
		if(bias != NULL)
		{
			for(int i = 0; i < layer->size(); ++i)
			{
				ViSynapseFactory::create(bias, layer->at(i));
			}
		}

		if(!success)
		{
			error = true;
		}
	}
	else
	{
		delete layer;
		layer = NULL;
		error = true;
	}
	return layer;
}

QList<ViNeuralLayer*> ViNeuralLayerFactory::create(ViElementList elements, bool &error)
{
	QList<ViNeuralLayer*> neurons;
	for(int i = 0; i < elements.size(); ++i)
	{
		neurons.append(create(elements[i], error));
	}
	return neurons;
}

ViNeuralLayer* ViNeuralLayerFactory::create(int neuronCount, ViNeuron::Type type, ViActivationFunction *activationFunction, double bias)
{
	ViNeuralLayer *layer = new ViNeuralLayer();
	if(type == ViNeuron::InputNeuron)
	{
		for(int i = 0; i < neuronCount; ++i)
		{
			layer->add(ViStaticNeuronFactory::create(type));
		}
	}
	else
	{
		for(int i = 0; i < neuronCount; ++i)
		{
			layer->add(ViNeuronFactory::create(type, activationFunction));
		}
	}
	if(bias != 0)
	{
		ViStaticNeuron *biasNeuron = ViStaticNeuronFactory::create(ViNeuron::BiasNeuron, bias);
		layer->setBias(biasNeuron);
		for(int i = 0; i < layer->size(); ++i)
		{
			ViSynapseFactory::create(biasNeuron, layer->at(i));
		}
	}
	return layer;
}

ViNeuralLayer* ViNeuralLayerFactory::createInput(int neuronCount)
{
	return create(neuronCount, ViNeuron::InputNeuron);
}

ViNeuralLayer* ViNeuralLayerFactory::createOutput(int neuronCount, ViActivationFunction *activationFunction, double bias)
{
	return create(neuronCount, ViNeuron::OutputNeuron, activationFunction, bias);
}

ViNeuralLayer* ViNeuralLayerFactory::createHidden(int neuronCount, ViActivationFunction *activationFunction, double bias)
{
	return create(neuronCount, ViNeuron::HiddenNeuron, activationFunction, bias);
}
