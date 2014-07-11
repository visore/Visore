#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <viserializer.h>
#include <vinoise.h>
#include <vibuffer.h>
#include <vilogger.h>
#include <vinotifier.h>
#include <vierror.h>

class ViInterpolator : public ViLibrary, public ViSerializer, public ViNotifier
{

	public:

		enum Direction
		{
			Forward,
			Backward,
			Bidirectional
		};

	public:

		ViInterpolator();
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		void setDirection(const Direction &direction);

		// If set to 0 or negative: will use as many samples as available
		virtual void setWindowSize(const int &size);
		int windowSize();

		void interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *mask, ViErrorCollection *modelErrors = NULL);
		// gapErrors = errors for the gaps we are interpolating, modelErrors = errors for how well our model fits the given/observed value (aka left and right samples)
		void interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *target, ViBuffer *sizeMask, ViErrorCollection *gapErrors, ViErrorCollection *modelErrors = NULL);
		void interpolate(ViSampleChunk &samples, const ViSampleChunk &noiseMask, const int &channel);

		virtual ViInterpolator* clone() = 0;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual void setParameter(const int &number, const qreal &value);
		void setParameter(const QString &name, const qreal &value);
		void setParameters(const qreal &parameter1);
		void setParameters(const qreal &parameter1, const qreal &parameter2);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5);

		virtual bool validParameters();
		virtual bool hasParameter(const QString &name);

		QString parameterName(const int &index, const bool &allCaps = true);

		virtual void initialize(const int &channelCount);

	protected:

		void interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *mask, ViErrorCollection *modelErrors, const bool &reversed, const qreal &progressScale, const qreal progressOffset);

		void adjustSamples(ViSampleChunk &data);
		void addParameterName(const QString &name);

		void process(const ViSampleChunk &samples, ViSampleChunk &output, const ViSampleChunk &noise, int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache, ViErrorCollection *modelErrors, const int &channel); // Returns the writable samples
		void interpolate(ViSampleChunk &output, const int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache, ViErrorCollection *modelErrors, const int &channel);

		virtual bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel) = 0;

	protected:

		Direction mDirection;
		int mWindowSize;
		int mHalfWindowSize;
		int mHalfWindowSizeLeft;
		int mHalfWindowSizeRight;
		QList<QString> mParameterNames;

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
