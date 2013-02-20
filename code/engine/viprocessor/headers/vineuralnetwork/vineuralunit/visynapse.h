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

		double value() const; // Weighted value of the input neuron
		
		void setInput(ViNeuron *neuron);
		void setOutput(ViNeuron *neuron);
		void setWeight(double weight);

		bool operator == (const ViSynapse &other) const;

	protected:

		ViSynapse();
		ViSynapse(ViNeuron *input, ViNeuron *output, double weight = 1);

	private:

		ViNeuron *mInput;
		ViNeuron *mOutput;
		double mWeight;

};

typedef QList<ViSynapse*> ViSynapseList;

#endif
