#ifndef VIPRIMITIVEINTERPOLATOR_H
#define VIPRIMITIVEINTERPOLATOR_H

#include <viinterpolator.h>

// http://paulbourke.net/miscellaneous/interpolation/

class ViPrimitiveInterpolator : public ViInterpolator
{

	public:

		enum Mode
		{
			Zero,	// Set all predictions to 0
			Random,	// Set all predictions to Gaussian white noise generated from the sample variance
			Cosine,	// Set all predictions via cosine interpolation
		};

	public:

		ViPrimitiveInterpolator(const Mode &mode = Zero);

		QString name(QString replace = "", bool spaced = false);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViPrimitiveInterpolator* clone();

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		void interpolateZero(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);
		void interpolateRandom(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);
		void interpolateCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);

	private:

		void (ViPrimitiveInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);

	private:

		Mode mMode;

};

#endif
