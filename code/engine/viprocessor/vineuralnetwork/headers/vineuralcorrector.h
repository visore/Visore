#ifndef VINEURALCORRECTOR_H
#define VINEURALCORRECTOR_H

#include <viprocessor.h>
#include <vineuralnetworkfactory.h>
#include <vitrainermanager.h>
#include <vitargetprovidermanager.h>
#include <QWaitCondition>

class ViNeuralCorrectorThread : public QThread
{

	Q_OBJECT

	signals:

		void outputAvailable();

	public:

		ViNeuralCorrectorThread(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider);
		~ViNeuralCorrectorThread();

		bool addData(ViSampleChunk *data);
		void setOffsets(int data, int targetLeft, int targetRight);

		void setOutputSize(int size);
		ViSampleChunk* output();
		bool hasOutput();

		void run();

		void stop();

	private:

		ViNeuralNetwork *mNetwork;
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		int mProviderLeftSamples;
		int mProviderRightSamples;

		QQueue<ViSampleChunk*> mData;
		ViSampleChunk mLeftTargetData;
		ViSampleChunk mRightTargetData;

		int mOutputSize;
		int mOutputSample;
		ViSampleChunk *mOutput;
		QQueue<ViSampleChunk*> mOutputs;
		QMutex mOutputMutex;

		bool mStop;
		int mDataOffset;
		int mTargetLeftOffset;
		int mTargetRightOffset;

		QMutex mMutex;

		QMutex mWaitMutex;
		QWaitCondition mWaitCondition;

};

class ViNeuralCorrector : public ViModifyProcessor
{

	Q_OBJECT

	private slots:

		bool writeOutput();

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

		int mProviderLeftSamples;
		int mProviderRightSamples;

		bool mSeparateChannels;

		ViNeuralNetworkFactory mFactory;
		QList<ViNeuralCorrectorThread*> mThreads;

		QVector<qreal> mReadBuffer;
		bool mFirstWrite;

		int mChannels;
		int mMinimumSamples;
		int mLeftSamples;
		int mWriteSamples;

		int mDataOffset;
		int mTargetLeftOffset;
		int mTargetRightOffset;

		int mCurrentWriteChannel;

};

#endif
