#ifndef VINEURALINTERPOLATOR_H
#define VINEURALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vifann.h>

class ViNeuralInterpolator : public ViInterpolator
{

	public:

		enum Mode
		{
			Incremental,	// TDANN trained incrementally with all succeeding samples. Max-noise-size samples all predicted at once.
			Recurrent,		// Jordan SRTDANN trained incrementally with all succeeding samples. One sample predicted at a time.
			SeparateBatch,	// TDANN trained with RProp with succeeding samples x saamples appart. One ANN per noise-size.
			MaximumBatch,	// TDANN trained with RProp with succeeding samples x saamples appart. Max-noise-size samples all predicted at once.
			Interpolation,	// Half the inputs from left, half from right. Trained with RProp to interpolate between both sides. One ANN per noise-size.
		};

	public:

		ViNeuralInterpolator(Mode mode = Interpolation);
		ViNeuralInterpolator(const ViNeuralInterpolator &other);
		~ViNeuralInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setWindowSize(const int &size); // Input neurons (size is sum of right and left samples)
		void setMode(Mode mode);
		void setEpochs(int epochs);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViNeuralInterpolator* clone();

		void printTrainMse();

	protected:

		void clear();
		void initialize(const int &channelCount);

		void initializeIncremental(const int &channelCount);
		void initializeRecurrent(const int &channelCount);
		void initializeSeparateBatch(const int &channelCount);
		void initializeMaximumBatch(const int &channelCount);
		void initializeInterpolation(const int &channelCount);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		bool interpolateIncremental(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateRecurrent(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateSeparateBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateMaximumBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateInterpolation(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		// trainCount: The number of training sets to use (includes left and right). Will always use the same number of training sets from left and right samples. If <= 0, use max number of sets
		// stepSize: How many samples to skip before creating the next training set. If this is too high, trainCount will be adjusted
		void train(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const int &trainCount, const int &stepSize);

		void trainSeparateBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize,  int trainCount, const int &stepSize);
		void trainMaximumBatch(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize,  int trainCount, const int &stepSize);
		bool devideBatch(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, qreal *output, const int &outputSize);

		bool devide(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, const int &rightInputs, qreal *output, const int &outputSize);
		void devide(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, qreal *input, const int &leftInputs, const int &rightInputs);

		void reverse(const qreal *input, qreal *output, const int &size);

	private:

		Mode mMode;
		QList<ViFann*> mNetworks;
		qreal *mInput;
		QList<qreal*> mOutput;

		QList<qreal> a;
		QList<qint64> b;
		QList<qint32> mHiddenNeurons;
		int l1,l2,l3,l4,l5;

		int mWindowSize;
		int mInputs;
		int mLeftInputs;
		int mRightInputs;
		int mOutputs;

		int mEpochs;

		int mTrainCount;

};

#endif
