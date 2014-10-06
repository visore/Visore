#ifndef VINEARESTNEIGHBOURINTERPOLATOR_H
#define VINEARESTNEIGHBOURINTERPOLATOR_H

#include <viinterpolator.h>

class ViNearestNeighbourInterpolator : public ViInterpolator
{

	public:

		enum Mode
		{
			Mean,
			Median,
			Traditional
		};

	public:

		ViNearestNeighbourInterpolator(const Mode &mode = Mean);
		ViNearestNeighbourInterpolator(const ViNearestNeighbourInterpolator &other);
		~ViNearestNeighbourInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setMode(const Mode &mode);
		void setK(const int &k); // Use the mean/median of the k-nearest samples

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViNearestNeighbourInterpolator* clone();

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

		bool interpolateMean(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateMedian(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateTraditional(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		inline void nearest(const int &leftSize, const int &rightSize, const int &outputSize, const int &x, const int &k, int &useLeft, int &useRight);

	private:

		bool (ViNearestNeighbourInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		Mode mMode;
		int mK;

};

#endif

