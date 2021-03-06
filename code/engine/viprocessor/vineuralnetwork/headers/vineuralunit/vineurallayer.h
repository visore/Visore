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

		int inputSize();
		int outputSize();

		int position(ViNeuron *neuron);
		ViNeuron* at(int index) const;
		ViNeuron* neuron(const QString id) const;

		ViNeuron* bias();
		bool setBias(ViNeuron *bias);
		bool hasBias() const;

		double value(int index);

		void setValue(int index, double value);

		void setActivationFunction(ViActivationFunction *activationFunction); // Takes ownership

		ViNeuron* operator [] (const int index) const;
		ViNeuron* operator [] (const QString id) const;
		bool operator == (const ViNeuralLayer &other) const;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);
		
	protected:

		ViNeuralLayer(ViNeuronList neurons = ViNeuronList());

	private:

		ViNeuronList mNeurons;
		ViNeuron *mBias;

};

typedef QList<ViNeuralLayer*> ViNeuralLayerList;

#endif
