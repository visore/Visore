#ifndef VICONSTANTPREDICTOR_H
#define VICONSTANTPREDICTOR_H

#include <vipredictor.h>

class ViConstantPredictor : public ViPredictor
{

	public:

		enum Mode
		{
			Zero,	// Set all predictions to 0
			Mean,	// Set all predictions to the mean of the input samples
			Last,	// Set all predictions to the last sample value of the input
			Random	// Set all predictions to Gaussian white noise generated from the sample variance
		};

	public:

		ViConstantPredictor(const Mode &mode = Last);
		ViConstantPredictor(const ViConstantPredictor &other);
		~ViConstantPredictor();

		QString name(QString replace = "", bool spaced = false);

		void setMode(const Mode &mode);

		void setParameter(const int &number, const qreal &value);

	protected:

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViErrorCollection *modelErrors = NULL);

		void predictZero(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		void predictMean(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		void predictLast(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		void predictRandom(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

	private:

		void (ViConstantPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

	private:

		Mode mMode;

};

#endif
