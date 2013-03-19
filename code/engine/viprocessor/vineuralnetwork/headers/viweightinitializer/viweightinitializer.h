#ifndef VIWEIGHTINITIALIZER_H
#define VIWEIGHTINITIALIZER_H

#include <vineuralnetwork.h>
#include <viserializer.h>

/*
	http://www.elen.ucl.ac.be/Proceedings/esann/esannpdf/es2001-6.pdf

	Weight Initialization Methods for Multilayer Feedforward
	Mercedes Fernández-Redondo and Carlos Hernández-Espinosa
	ESANN'2001 proceedings - European Symposium on Artificial Neural Networks
	Bruges (Belgium), 25-27 April 2001, D-Facto public., ISBN 2-930307-01-3, pp. 119-124
*/

class ViWeightInitializer : public ViSerializer
{

	public:

		ViWeightInitializer();
		virtual ~ViWeightInitializer();

		/*
			Returns the position of the current synapse in the network.
			Ranges from 0 to s-1, where s is the total number of synapses in the network.
		*/
		const int& currentSynapse() const;

		/*
			Returns the position of the current neuron in the network that serves as input to the current synapse.
			Ranges from 0 to n-1, where n is the total number of neurons in the network.
		*/
		const int& currentInputNeuron() const;

		/*
			Returns the position of the current neuron in the network that serves as output to the current synapse.
			Ranges from 0 to n-1, where n is the total number of neurons in the network.
		*/
		const int& currentOutputNeuron() const;

		/*
			Returns the position of the layer in the network that serves as input layer to the current synapse.
			Ranges from 0 to l-1, where l is the total number of layers in the network.
		*/
		const int& currentInputLayer() const;

		/*
			Returns the position of the layer in the network that serves as output layer to the current synapse.
			Ranges from 0 to l-1, where l is the total number of layers in the network.
		*/
		const int& currentOutputLayer() const;

		/*
			Returns the position of the neuron in the layer that serves as input to the current synapse.
			Ranges from 0 to nl-1, where nl is the total number of neurons in the current input layer l.
		*/
		const int& currentInputLayerNeuron() const;

		/*
			Returns the position of the neuron in the layer that serves as output to the current synapse.
			Ranges from 0 to nl-1, where nl is the total number of neurons in the current output layer l.
		*/
		const int& currentOutputLayerNeuron() const;

		/*
			Returns the activation function for the neuron that serves as input to the current synapse.
		*/
		const ViActivationFunction* currentInputActivationFunction() const;

		/*
			Returns the activation function for the neuron that serves as output to the current synapse.
		*/
		const ViActivationFunction* currentOutputActivationFunction() const;

		void initialize(ViNeuralNetwork *network);

		static ViWeightInitializer* defaultInitializer();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		virtual void initialize(ViSynapse *synapse) = 0;

	private:

		void initialize(ViNeuralNetwork *network, ViNeuralLayer *inputLayer, ViNeuron *neuron = NULL);

		int layerPosition(ViNeuralNetwork *network, int layerPosition, ViNeuron *neuron);
		int networkPosition(ViNeuralNetwork *network, ViNeuron *neuron);

	private:

		int mCurrentSynapse;
		int mCurrentInputNeuron;
		int mCurrentOutputNeuron;
		int mCurrentInputLayer;
		int mCurrentOutputLayer;
		int mCurrentInputLayerNeuron;
		int mCurrentOutputLayerNeuron;
		ViActivationFunction *mCurrentInputActivationFunction;
		ViActivationFunction *mCurrentOutputActivationFunction;

};

#endif
