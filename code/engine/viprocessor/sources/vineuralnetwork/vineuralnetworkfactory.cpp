#include <vineuralnetworkfactory.h>
#include <vineurallayerfactory.h>
#include <visynapsefactory.h>

ViNeuralNetwork* ViNeuralNetworkFactory::create(int layers, int neurons, ViActivationFunction *activationFunction)
{
	ViIntegerList list;
	for(int i = 0; i < layers; ++i)
	{
		list.append(neurons);
	}
	return ViNeuralNetworkFactory::create(list, activationFunction);
}

ViNeuralNetwork* ViNeuralNetworkFactory::create(ViIntegerList layers, ViActivationFunction *activationFunction)
{
	ViNeuralNetwork *network = new ViNeuralNetwork();
	
	for(int i = 0; i < layers.size(); ++i)
	{
		network->add(ViNeuralLayerFactory::create(layers[i], activationFunction));
	}

	for(int i = 0; i < network->size() - 1; ++i)
	{
		for(int j = 0; j < network[i].size() - 1; ++j)
		{
			for(int k = 0; k < network[i + 1].size() - 1; ++k)
			{
				ViSynapseFactory::create(network->at(i)->at(j), network->at(i + 1)->at(k));
			}
		}
	}

	return network;
}
