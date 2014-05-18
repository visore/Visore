#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <viserializer.h>
#include <vinoise.h>
#include <vibuffer.h>
#include <vilogger.h>

class ViInterpolator : public ViLibrary, public ViSerializer
{

	public:

		ViInterpolator();
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		// If set to 0 or negative: will use as many samples as available
		virtual void setWindowSize(const int &size);
		int windowSize();

		bool interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *mask);
		bool interpolate(ViSampleChunk &samples, const ViSampleChunk &noiseMask, const int &channel);

		virtual ViInterpolator* clone() = 0;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual void setParameters(const qreal &param1){ LOG("Invalid number of parameters given: 1"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2){ LOG("Invalid number of parameters given: 2"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3){ LOG("Invalid number of parameters given: 3"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3, const qreal &param4){ LOG("Invalid number of parameters given: 4"); exit(-1); }

	protected:

		void process(const ViSampleChunk &samples, ViSampleChunk &output, const ViSampleChunk &noise, int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache); // Returns the writable samples
		void interpolate(ViSampleChunk &output, const int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache);
		virtual bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize) = 0;

	protected:

		int mWindowSize;
		int mHalfWindowSize;

};

class ViDegreeInterpolator : public ViInterpolator
{

	public:

		ViDegreeInterpolator();
		ViDegreeInterpolator(const int &degree);
		ViDegreeInterpolator(const ViDegreeInterpolator &other);
		virtual ~ViDegreeInterpolator();

		void setDegree(const int &degree);
		int degree() const;

	protected:

		int mDegree;

};

#endif
