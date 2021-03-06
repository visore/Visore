#include <vibackpropagationtrainer.h>
#include <vierrorfunctionmanager.h>

ViBackpropagationTrainer::ViBackpropagationTrainer()
	: ViTrainer()
{
}

ViBackpropagationTrainer::ViBackpropagationTrainer(const ViBackpropagationTrainer &other)
	: ViTrainer(other)
{
}

ViBackpropagationTrainer::~ViBackpropagationTrainer()
{
}

ViTrainer* ViBackpropagationTrainer::clone()
{
	return new ViBackpropagationTrainer(*this);
}

void ViBackpropagationTrainer::trainNetwork()
{
	const ViRealList &targets = targetValues();
	ViNeuralNetwork *currentNetwork = network();

	ViNeuralLayer *currentLayer;
	ViNeuron *currentNeuron;
	
	//Change the output weights
	currentLayer = currentNetwork->at(currentNetwork->size() - 1);
	for(int i = 0; i < currentLayer->size(); ++i)
	{
		currentNeuron = currentLayer->at(i);
		calculateError(currentNeuron, targets.at(i));
		adaptWeights(currentNeuron);
	}

	//Change hidden weights
	for(int k = currentNetwork->size() - 2; k > 0; --k)
	{
		currentLayer = currentNetwork->at(k);
		for(int i = 0; i < currentLayer->size(); ++i)
		{
			currentNeuron = currentLayer->at(i);
			calculateError(currentNeuron);
			adaptWeights(currentNeuron);
		}
	}
}

void ViBackpropagationTrainer::calculateError(ViNeuron *neuron, const qreal &targetValue)
{
	neuron->setError((targetValue - neuron->value()) * neuron->activationFunction()->calculateDerivative(neuron->value()));
}

void ViBackpropagationTrainer::calculateError(ViNeuron *neuron)
{
	qreal error = 0;
	for(int i = 0; i < neuron->outputSize(); ++i)
	{
		error += neuron->outputAt(i)->output()->error() * neuron->outputAt(i)->weight() * neuron->activationFunction()->calculateDerivative(neuron->value());
	}
	neuron->setError(error);
}

void ViBackpropagationTrainer::adaptWeights(ViNeuron *neuron)
{
	ViSynapse *synapse;
	qreal weightAdjustment;
	for(int j = 0; j < neuron->inputSize(); ++j)
	{
		synapse = neuron->inputAt(j);
		weightAdjustment = learningRate() * neuron->error() * synapse->input()->value();
		synapse->setWeight(synapse->weight() + weightAdjustment);
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

ViTrainer* create()
{
	return new ViBackpropagationTrainer();
}

#ifdef __cplusplus
}
#endif
