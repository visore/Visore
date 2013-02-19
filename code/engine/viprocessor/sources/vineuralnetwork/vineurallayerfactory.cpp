#include <vineurallayerfactory.h>
#include <vineuronfactory.h>

ViNeuralLayer* ViNeuralLayerFactory::create(int neuronCount, ViActivationFunction *activationFunction)
{
	ViNeuralLayer *layer = new ViNeuralLayer();
	for(int i = 0; i < neuronCount; ++i)
	{
		layer->add(ViNeuronFactory::create(activationFunction));
	}
	return layer;
}
