#ifndef VINEURALNETWORK_H
#define VINEURALNETWORK_H

#include <vineuron.h>
#include <vineurallayer.h>
#include <QRunnable>

class ViNeuralNetwork : public ViNeuralUnit, public QRunnable
{

	friend class ViNeuralNetworkFactory;

	public:

		ViNeuralNetwork(const ViNeuralNetwork &other);
		~ViNeuralNetwork();

		void run();

		bool add(ViNeuralLayer *layer);

		void clear();

		bool contains(ViNeuralLayer *layer) const;

		int size() const;

		ViNeuralLayer* at(int index) const;
		ViNeuron* neuron(const QString id) const;

		void setInput(int index, double value);
		void setInputs(ViDoubleList values);

		double output(int index = 0);
		ViDoubleList outputs();

		int inputCount();
		int outputCount();

		ViNeuralLayer* operator [] (const int index) const;
		bool operator == (const ViNeuralNetwork &other) const;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);
		
	protected:

		ViNeuralNetwork(ViNeuralLayerList layers = ViNeuralLayerList());

	private:

		ViNeuralLayerList mLayers;

};

#endif
