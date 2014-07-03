#ifndef VINEURALPREDICTOR_H
#define VINEURALPREDICTOR_H

#include <vipredictor.h>
#include <vifann.h>

class ViNeuralPredictor : public ViPredictor
{

	public:

		ViNeuralPredictor();
		ViNeuralPredictor(const ViNeuralPredictor &other);
		~ViNeuralPredictor();

		void setWindowSize(const int &size);

		void setParameter(const int &number, const qreal &value);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0);

		void initialize(const int &channelCount, const int &predictionCount);

	protected:

		void clear();

	private:

		QList<ViFann*> mNetworks;
		int mInputs;

		int l1,l2,l3,l4,l5;

};

#endif
