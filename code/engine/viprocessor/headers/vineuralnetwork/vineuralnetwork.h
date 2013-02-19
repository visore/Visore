#ifndef VINEURALNETWORK_H
#define VINEURALNETWORK_H

#include <vineuron.h>
#include <vineurallayer.h>

class ViNeuralNetwork : public ViNeuralUnit
{

	friend class ViNeuralNetworkFactory;

	public:

		ViNeuralNetwork(const ViNeuralNetwork &other);
		~ViNeuralNetwork();

		bool add(ViNeuralLayer *layer);

		void clear();

		bool contains(ViNeuralLayer *layer) const;

		int size() const;

		ViNeuralLayer* at(int index) const;

		ViNeuralLayer* operator [] (const int index) const;
		bool operator == (const ViNeuralNetwork &other) const;
		
	protected:

		ViNeuralNetwork(ViNeuralLayerList layers = ViNeuralLayerList());

	private:

		ViNeuralLayerList mLayers;

};

#endif
