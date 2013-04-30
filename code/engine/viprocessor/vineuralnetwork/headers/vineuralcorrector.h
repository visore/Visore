#ifndef VINEURALCORRECTOR_H
#define VINEURALCORRECTOR_H

#include <viprocessor.h>
#include <vineuralnetworkfactory.h>
#include <vitrainermanager.h>
#include <vitargetprovidermanager.h>

class ViNeuralCorrectorThread : public QThread
{

	public:

		ViNeuralCorrectorThread(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider);
		~ViNeuralCorrectorThread();

		void setData(ViSampleChunk *data);

		ViNeuralNetwork* network();

		void run();

	private:

		ViNeuralNetwork *mNetwork;
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		ViSampleChunk *mData;
		ViSampleChunk mTargetData;
		int mTargetOffset;

};

class ViNeuralCorrector : public ViModifyProcessor
{

	public:

		ViNeuralCorrector();
		~ViNeuralCorrector();

		//If enabled, every channel will be processed with its own NN
		void enableSeparateChannels(bool enable = true);
		void disableSeparateChannels(bool disable = true);

		void initialize();
		void execute();
		void finalize();

	private:

		void (ViNeuralCorrector::*executePointer)();
		void executeWithChannels();
		void executeWithoutChannels();

	private:

		bool mSeparateChannels;

		ViNeuralNetworkFactory mFactory;
		QList<ViNeuralCorrectorThread*> mThreads;

		QVector<qreal> mReadBuffer;
		ViSampleChunk mWriteBuffer;
		bool mFirstWrite;

		int mChannels;
		int mHalfTargetSamples;
		int mMinimumSamples;
		int mWriteSamples;

};

#endif
