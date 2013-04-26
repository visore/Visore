#ifndef VINEURALCORRECTOR_H
#define VINEURALCORRECTOR_H

#include <viprocessor.h>
#include <vineuralnetworkfactory.h>
#include <vitrainermanager.h>
#include <vitargetprovidermanager.h>

class ViNeuralCorrector : public ViModifyProcessor
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
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		ViRealList mReadBuffer;
		ViSampleChunk mWriteBuffer;
		bool mFirstWrite;

};

#endif
