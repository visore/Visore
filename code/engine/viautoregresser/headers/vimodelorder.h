#ifndef VIMODELORDER_H
#define VIMODELORDER_H

class ViModelOrder
{

	public:

		ViModelOrder();
		ViModelOrder(const ViModelOrder &other);
		virtual ~ViModelOrder();

		void setParameterCount(int parameters);

		virtual int order() = 0;

};

// Audacity - InterpolateAudio.cpp line 112
class ViMazzoniModelOrder : public ViModelOrder
{

	public:

		ViMazzoniModelOrder();
		ViMazzoniModelOrder(const ViMazzoniModelOrder &other);

		void setSampleCount(const int &count);
		void setBadSampleCount(const int &count);
		void setFirstBadSample(const int &index);

		int order();

		static int order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample);

	private:

		int mSampleCount;
		int mBadSampleCount;
		int mFirstBadSample;

};

/*
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

		void setSampleCount(const int &count);
		void setBadSampleCount(const int &count);
		void setFirstBadSample(const int &index);

		int order();

		static int order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample);

	private:

		int mSampleCount;
		int mBadSampleCount;
		int mFirstBadSample;

};

*/
#endif
