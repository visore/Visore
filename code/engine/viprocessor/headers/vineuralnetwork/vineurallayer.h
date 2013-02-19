#ifndef VINEURALLAYER_H
#define VINEURALLAYER_H

#include <vineuron.h>

class ViNeuralLayer : public ViNeuralUnit
{

	friend class ViNeuralLayerFactory;

	public:

		ViNeuralLayer(const ViNeuralLayer &other);
		~ViNeuralLayer();

		bool add(ViNeuron *neuron);

		void clear();

		bool contains(ViNeuron *neuron) const;

		int size() const;

		ViNeuron* at(int index) const;

		ViNeuron* operator [] (const int index) const;
		bool operator == (const ViNeuralLayer &other) const;
		
	protected:

		ViNeuralLayer(ViNeuronList neurons = ViNeuronList());

	private:

		ViNeuronList mNeurons;

};

typedef QList<ViNeuralLayer*> ViNeuralLayerList;

#endif
