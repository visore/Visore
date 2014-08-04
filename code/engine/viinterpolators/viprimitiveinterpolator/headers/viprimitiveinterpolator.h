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
			Duplicate, // Duplicates the last outputSize samples over the gap
			Mirror, // Takes the mirror of the last outputSize samples
			Similarity, // Uses the sequence of similar samples elsewere
			Lanczos,
		};

	public:

		ViPrimitiveInterpolator(const Mode &mode = Zero);

		QString name(QString replace = "", bool spaced = false);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViPrimitiveInterpolator* clone();

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		void interpolateZero(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateRandom(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateMirror(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateDuplicate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateSimilarity(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);
		void interpolateLanczos(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);

		void calculateDifferences(const qreal *samples, const int &size,  qreal *outputSamples, const int &outputSize, qreal *differences, qreal &lastSample);
		void similarity(const qreal *samples, const int &size, const qreal *differences, qreal &bestScore, qreal &bestDifference);

	private:

		void (ViPrimitiveInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error);

	private:

		Mode mMode;
		int mSamples;

};

#endif
