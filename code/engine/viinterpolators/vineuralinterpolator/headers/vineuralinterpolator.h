#ifndef VINEURALINTERPOLATOR_H
#define VINEURALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vifann.h>

class ViNeuralInterpolator : public ViInterpolator
{

	public:

		ViNeuralInterpolator();
		ViNeuralInterpolator(const ViNeuralInterpolator &other);
		~ViNeuralInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setWindowSize(const int &size); // Input neurons (size is sum of right and left samples)

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViNeuralInterpolator* clone();

	protected:

		void clear();
		void initialize(const int &channelCount);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		// trainCount: The number of training sets to use (includes left and right). Will always use the same number of training sets from left and right samples. If <= 0, use max number of sets
		// stepSize: How many samples to skip before creating the next training set. If this is too high, trainCount will be adjusted
		void train(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const int &trainCount, const int &stepSize);

		bool devide(const qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &leftInputs, const int &rightInputs, qreal *output, const int &outputSize);
		void devide(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, qreal *input, const int &leftInputs, const int &rightInputs);

		void reverse(const qreal *input, qreal *output, const int &size);

	private:

		QList<ViFann*> mNetworks;
		qreal *mInput;
		QList<qreal*> mOutput;

		QList<qreal> a;
		QList<qint64> b;
		int l1,l2,l3,l4,l5;

		int mWindowSize;
		int mInputs;
		int mLeftInputs;
		int mRightInputs;
		int mOutputs;

		int mTrainCount;

};

#endif
