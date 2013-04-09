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

void ViBackpropagationTrainer::setErrorFunction(ViErrorFunction *function)
{
	if(function->name() != "SumOfSquaredErrorFunction")
	{
		LOG("Backpropagation requires the SumOfSquaredErrorFunction. The error function will be set as required.");
		delete function;
		function = ViErrorFunctionManager::create("SumOfSquaredErrorFunction");
	}
	ViTrainer::setErrorFunction(function);
}

ViTrainer* ViBackpropagationTrainer::clone()
{
	return new ViBackpropagationTrainer(*this);
}

void ViBackpropagationTrainer::trainNetwork()
{
	ViRealList targetValues = {0.5};//****************
	ViNeuralNetwork *currentNetwork = network();

	ViNeuralLayer *currentLayer;
	ViNeuron *currentNeuron;
	
	//Change the output weights
	currentLayer = currentNetwork->at(currentNetwork->size() - 1);
	for(int i = 0; i < currentLayer->size(); ++i)
	{
		currentNeuron = currentLayer->at(i);
		calculateError(currentNeuron, targetValues.at(i));
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
