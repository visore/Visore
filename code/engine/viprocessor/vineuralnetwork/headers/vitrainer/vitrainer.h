#ifndef VITRAINER_H
#define VITRAINER_H

#include <viserializer.h>
#include <vineuralnetwork.h>
#include <vierrorfunction.h>

class ViTrainer : public ViSerializer
{

	public:

		ViTrainer();
		ViTrainer(const ViTrainer &other);
		virtual ~ViTrainer();

		void clear();

		ViNeuralNetwork* network();
		void setNetwork(ViNeuralNetwork *network);

		int iterationLimit();
		void setIterationLimit(int limit);

		qreal errorLimit();
		void setErrorLimit(qreal limit);

		int currentIteration();
		qreal currentError();

		void setLearningRate(qreal learningRate);
		qreal learningRate();

		virtual void addErrorFunction(ViErrorFunction *function);

		void train();
		void train(ViNeuralNetwork *network);
		void train(ViNeuralNetwork *network, int iterationLimit, qreal errorLimit);

		//With a single iteration
		void trainSingle();
		void trainSingle(ViNeuralNetwork *network);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViTrainer* clone() = 0;

	protected:

		void calculateError();
		virtual void trainNetwork() = 0;
		
	private:

		ViNeuralNetwork *mNetwork;
		QList<ViErrorFunction*> mErrorFunctions;

		qreal mLearningRate;

		int mIterationLimit;
		qreal mErrorLimit;

		int mCurrentIteration;
		qreal mCurrentError;

		ViRealMatrix mErrors;

};

#endif
