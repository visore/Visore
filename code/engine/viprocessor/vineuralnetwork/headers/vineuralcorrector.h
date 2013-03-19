#ifndef VINEURALCORRECTOR_H
#define VINEURALCORRECTOR_H

#include <viprocessor.h>
#include <vineuralnetworkfactory.h>

class ViNeuralCorrector : public ViProcessor
{

	public:

		ViNeuralCorrector();
		~ViNeuralCorrector();

		void initialize();
		void execute();
		void finalize();

	private:

		ViNeuralNetworkFactory mFactory;
		ViNeuralNetwork *mNetwork;

};

#endif
