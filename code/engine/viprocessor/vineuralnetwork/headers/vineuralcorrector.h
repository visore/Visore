#ifndef VINEURALCORRECTOR_H
#define VINEURALCORRECTOR_H

#include <viprocessor.h>
#include <vineuralnetwork.h>
#include <vitrainermanager.h>
#include <vitargetprovidermanager.h>

class ViNeuralCorrector : public ViModifyProcessor
{

	public:

		ViNeuralCorrector();
		ViNeuralCorrector(ViNeuralNetwork *network, ViTrainer *trainer, ViTargetProvider *provider); //Takes ownership
		~ViNeuralCorrector();

		void initialize();
		void execute(const int &channel);
		void finalize();

        ViElement exportData();
        bool importData(ViElement element);

	private:

		QMutex mMutex;

		ViNeuralNetwork *mNetwork;
		ViTrainer *mTrainer;
		ViTargetProvider *mProvider;

		QList<ViNeuralNetwork*> mNetworks;
		QList<ViTrainer*> mTrainers;
		QList<ViTargetProvider*> mProviders;

		QVector<QVector<qreal>> mReadBuffers;
		QVector<bool> mFirstWrites;

		QVector<ViSampleChunk> mWriteBuffers;
		QVector<int> mWritePositions;

		int mMinimumSamples;
		int mLeftSamples;

		int mDataOffset;
		int mTargetLeftOffset;
		int mTargetRightOffset;

		int mProviderLeftSamples;
		int mProviderRightSamples;


};

#endif
