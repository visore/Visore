#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <viserializer.h>
#include <vinoise.h>

class ViInterpolator : public ViLibrary, public ViSerializer
{

	public:

		ViInterpolator();
		ViInterpolator(const int &maximumSamples);
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		// The maximum number of samples that will be used for the interpolation on both sides
		// If set to 10, not more than 10 left and 10 right samples will be used
		// If set to 0 or negative: will use as many samples as available
		void setMaximumSamples(const int &count);
		int maximumSamples() const;

		bool interpolate(ViSampleChunk &samples, const ViNoise &noise);

		virtual ViInterpolator* clone() = 0;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize) = 0;

	private:

		int mMaximumSamples;

};

class ViDegreeInterpolator : public ViInterpolator
{

	public:

		ViDegreeInterpolator();
		ViDegreeInterpolator(const int &degree);
		ViDegreeInterpolator(const int &maximumSamples, const int &degree);
		ViDegreeInterpolator(const ViDegreeInterpolator &other);
		virtual ~ViDegreeInterpolator();

		void setDegree(const int &degree);
		int degree() const;

	protected:

		int mDegree;

};

#endif
