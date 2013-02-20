#ifndef VINEURALLAYER_H
#define VINEURALLAYER_H

#include <vineuron.h>
#include <QRunnable>

class ViNeuralLayer : public ViNeuralUnit, public QRunnable
{

	friend class ViNeuralLayerFactory;

	public:

		ViNeuralLayer(const ViNeuralLayer &other);
		~ViNeuralLayer();

		void run();

		bool add(ViNeuron *neuron);

		void clear();

		bool contains(ViNeuron *neuron) const;

		int size() const;

		ViNeuron* at(int index) const;

		double value(int index);

		void setValue(int index, int value);

		ViNeuron* operator [] (const int index) const;
		bool operator == (const ViNeuralLayer &other) const;
		
	protected:

		ViNeuralLayer(ViNeuronList neurons = ViNeuronList());

	private:

		ViNeuronList mNeurons;

};

typedef QList<ViNeuralLayer*> ViNeuralLayerList;

#endif
