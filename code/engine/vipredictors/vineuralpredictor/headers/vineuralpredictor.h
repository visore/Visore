#ifndef VINEURALPREDICTOR_H
#define VINEURALPREDICTOR_H

#include <vipredictor.h>
#include <vifann.h>

class ViNeuralPredictor : public ViPredictor
{

	public:

		enum Mode
		{
			IncrementalSet,
			IncrementalRecurrent,
		};

	public:

		ViNeuralPredictor(Mode mode = IncrementalSet);
		ViNeuralPredictor(const ViNeuralPredictor &other);
		~ViNeuralPredictor();

		void setMode(Mode mode);
		void setWindowSize(const int &size);

		void setParameter(const int &number, const qreal &value);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0);

		void initialize(const int &channelCount, const int &predictionCount);

	protected:

		void initializeIncrementalSet(const int &channelCount, const int &predictionCount);
		void initializeIncrementalRecurrent(const int &channelCount, const int &predictionCount);

		bool predictIncrementalSet(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel);
		bool predictIncrementalRecurrent(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel);

		void clear();

	private:

		ViFann* mNetwork1;
		ViFann* mNetwork2;
		QList<qint32> mHiddenNeurons;
		int mInputs;
		int mOutputs;
		Mode mMode;

		qreal *mInput;
		qreal* mOutput;

};

#endif
