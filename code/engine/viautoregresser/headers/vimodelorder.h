#ifndef VIMODELORDER_H
#define VIMODELORDER_H

#include <QQueue>

class ViModelOrder
{

	public:

		ViModelOrder();
		ViModelOrder(const ViModelOrder &other);
		virtual ~ViModelOrder();

		virtual void clear();

		void setParameterCount(const int &parameters);
		void setSampleCount(const int &count);

		virtual int order() const = 0;

	protected:

		int mParameterCount;
		int mSampleCount;

};

// Audacity - InterpolateAudio.cpp line 112
class ViMazzoniModelOrder : public ViModelOrder
{

	public:

		ViMazzoniModelOrder();
		ViMazzoniModelOrder(const ViMazzoniModelOrder &other);
		virtual ~ViMazzoniModelOrder();

		virtual void clear();

		void setBadSampleCount(const int &count);
		void setFirstBadSample(const int &index);

		int order() const;

		static int order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample);

	protected:

		int mBadSampleCount;
		int mFirstBadSample;

};

// Akaike Information Criterion
// http://machinelearning102.pbworks.com/w/file/fetch/47699411/aic_reg.pdf
// http://pages.stern.nyu.edu/~churvich/TimeSeries/Handouts/AICC.pdf
// http://users.ecs.soton.ac.uk/jn2/teaching/aic.pdf
// http://www.brianomeara.info/tutorials/aic
// http://www.unc.edu/courses/2008fall/ecol/563/001/docs/lectures/lecture10.htm
// http://128.118.178.162/eps/comp/papers/0404/0404001.pdf

class ViAicModelOrder : public ViModelOrder
{

	public:

		ViAicModelOrder();
		ViAicModelOrder(const ViAicModelOrder &other);
		virtual ~ViAicModelOrder();

		virtual void clear();

		void setRss(const qreal &rss);
		void addRss(const qreal &predictedValue, const qreal &realValue); //Continuesly adds new values and calculates the RSS. Reset with clear()

		int order() const;

		static int order(const int &sampleCount, const int &parameterCount, const qreal &rss);

	protected:

		qreal mRss;
		QQueue<qreal> mPredictedValues;
		QQueue<qreal> mRealValues;

};

class ViAiccModelOrder : public ViAicModelOrder
{

	public:

		ViAiccModelOrder();
		ViAiccModelOrder(const ViAiccModelOrder &other);
		virtual ~ViAiccModelOrder();

		static int order(const int &sampleCount, const int &parameterCount, const qreal &rss);

};

#endif
