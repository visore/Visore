#include <vineurallayerfactory.h>
#include <vineuronfactory.h>
#include <vistaticneuronfactory.h>

ViNeuralLayer* ViNeuralLayerFactory::create(int neuronCount, ViActivationFunction *activationFunction, double bias)
{
	ViNeuralLayer *layer = new ViNeuralLayer();
	for(int i = 0; i < neuronCount; ++i)
	{
		layer->add(ViNeuronFactory::create(activationFunction));
	}
	if(bias != 0)
	{
		layer->add(ViStaticNeuronFactory::create(bias));
	}
	return layer;
}
