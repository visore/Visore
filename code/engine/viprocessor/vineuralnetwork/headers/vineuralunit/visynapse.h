#ifndef VISYNAPSE_H
#define VISYNAPSE_H

#include <vineuralunit.h>

#define DEFAULT_SYNAPSE_WEIGHT 0.5

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

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		ViSynapse();
		ViSynapse(ViNeuron *input, ViNeuron *output, double weight = DEFAULT_SYNAPSE_WEIGHT);

	private:

		ViNeuron *mInput;
		ViNeuron *mOutput;
		double mWeight;

};

typedef QList<ViSynapse*> ViSynapseList;

#endif
