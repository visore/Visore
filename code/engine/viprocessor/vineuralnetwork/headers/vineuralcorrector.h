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
		void setOffsets(int data, int targetLeft, int targetRight);

		ViNeuralNetwork* network();

		void run();

	private:

		ViNeuralNetwork *mNetwork;
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		ViSampleChunk *mData;
		ViSampleChunk mLeftTargetData;
		ViSampleChunk mRightTargetData;

		int mDataOffset;
		int mTargetLeftOffset;
		int mTargetRightOffset;

};

class ViNeuralCorrector : public ViModifyProcessor
{

	public:

		ViNeuralCorrector();
		ViNeuralCorrector(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider); //Takes ownership
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

		ViNeuralNetwork *mNetwork;
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		bool mSeparateChannels;

		ViNeuralNetworkFactory mFactory;
		QList<ViNeuralCorrectorThread*> mThreads;

		QVector<qreal> mReadBuffer;
		ViSampleChunk mWriteBuffer;
		bool mFirstWrite;

		int mChannels;
		int mMinimumSamples;
		int mLeftSamples;
		int mWriteSamples;

		int mDataOffset;
		int mTargetLeftOffset;
		int mTargetRightOffset;

};

#endif
