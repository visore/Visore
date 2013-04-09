#ifndef VIBACKPROPAGATIONTRAINER_H
#define VIBACKPROPAGATIONTRAINER_H

#include <vitrainer.h>

/*
	http://www4.rgu.ac.uk/files/chapter3%20-%20bp.pdf
	Page 19
*/

class ViBackpropagationTrainer : public ViTrainer
{

	public:

		ViBackpropagationTrainer();
		ViBackpropagationTrainer(const ViBackpropagationTrainer &other);
		~ViBackpropagationTrainer();

		void setErrorFunction(ViErrorFunction *function);

		ViTrainer* clone();

	protected:

		void trainNetwork();
		void calculateError(ViNeuron *neuron, const qreal &targetValue);
		void calculateError(ViNeuron *neuron);
		void adaptWeights(ViNeuron *neuron);

};

#endif
