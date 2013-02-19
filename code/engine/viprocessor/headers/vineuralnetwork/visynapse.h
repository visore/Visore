#ifndef VISYNAPSE_H
#define VISYNAPSE_H

#include <vineuralunit.h>

class ViNeuron;
class ViSynapseFactory;

class ViSynapse : public ViNeuralUnit
{

	friend class ViSynapseFactory;

	public:

		ViSynapse(const ViSynapse &other);

		ViNeuron* input() const;
		ViNeuron* output() const;
		double weight() const;
		
		void setInput(ViNeuron *neuron);
		void setOutput(ViNeuron *neuron);
		void setWeight(double weight);

		bool operator == (const ViSynapse &other) const;

	protected:

		ViSynapse(ViNeuron *input = NULL, ViNeuron *output = NULL, double weight = 1);

	private:

		ViNeuron *mInput;
		ViNeuron *mOutput;
		double mWeight;

};

typedef QList<ViSynapse*> ViSynapseList;

#endif
