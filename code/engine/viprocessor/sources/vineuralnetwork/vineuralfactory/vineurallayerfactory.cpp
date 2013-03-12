#include <vineurallayerfactory.h>
#include <vineuronfactory.h>
#include <vistaticneuronfactory.h>
#include <visynapsefactory.h>

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
