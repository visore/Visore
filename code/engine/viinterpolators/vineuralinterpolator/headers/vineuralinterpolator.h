#ifndef VINEURALINTERPOLATOR_H
#define VINEURALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vifann.h>

class ViNeuralInterpolator : public ViInterpolator
{

	public:

		ViNeuralInterpolator(ViFann *network); // Takes ownership
		ViNeuralInterpolator(const ViNeuralInterpolator &other);
		~ViNeuralInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setWindowSize(const int &size); // Input neurons (size is sum of right and left samples)

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);

		bool devide(qreal *samples, const int &sampleSize, const int &offset, qreal *input, const int &windowSize, qreal *output, const int &outputSize);

	private:

		ViFann *mNetwork;
		qreal *mInput;
		qreal *mOutput;

		int mWindowSize;

};

#endif
