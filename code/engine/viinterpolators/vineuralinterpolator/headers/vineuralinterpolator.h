#ifndef VINEURALINTERPOLATOR_H
#define VINEURALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vifann.h>

class ViNeuralInterpolator : public ViInterpolator
{

	public:

		enum Mode
		{
			IncrementalPrediction,				// Incrementally trains the data and then predicts all samples at once
			SetPrediction,						// Trains the NN with all data before the gap and then predicts all samples at once
			IncrementalRecurrentPrediction,		// Same as IncrementalPrediction, but uses a recurrent NN and predicts one sample at a time
			IncrementalSetPrediction,			// Same as SetPrediction, but uses a recurrent NN and predicts one sample at a time
			Interpolation						// Trains NN with all data surrounding it and interpolates all samples at once - BEST (hopefully)
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

		void initializeIncrementalPrediction(const int &channelCount);
		void initializeSetPrediction(const int &channelCount);
		void initializeIncrementalRecurrentPrediction(const int &channelCount);
		void initializeIncrementalSetPrediction(const int &channelCount);
		void initializeInterpolation(const int &channelCount);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		bool interpolateIncrementalPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateSetPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateIncrementalRecurrentPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateIncrementalSetPrediction(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);
		bool interpolateInterpolation(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		// trainCount: The number of training sets to use (includes left and right). Will always use the same number of training sets from left and right samples. If <= 0, use max number of sets
		// stepSize: How many samples to skip before creating the next training set. If this is too high, trainCount will be adjusted
		void train(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const int &trainCount, const int &stepSize);

		void trainSet(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize,  int trainCount, const int &stepSize);
		bool devideSet(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, qreal *output, const int &outputSize);

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
