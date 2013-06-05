#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <vichunk.h>
#include <qmath.h>

class ViInterpolator : public ViLibrary
{

	public:

		ViInterpolator(int leftSamples = 0, int rightSamples = 0);
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		virtual int leftSamples();
		virtual int rightSamples();
		virtual void setLeftSamples(int samples);
		virtual void setRightSamples(int samples);

		virtual void clear();

		virtual void setData(ViSampleChunk *left, ViSampleChunk *right);
		virtual void setLeftData(ViSampleChunk *data);
		virtual void setRightData(ViSampleChunk *data);

		virtual void setData(qreal left, qreal right);
		virtual void setLeftData(qreal data);
		virtual void setRightData(qreal data);

		virtual qreal calculate() = 0;
		virtual ViSampleChunk calculate(int samples) = 0; //Number of samples to interpolate

		virtual ViInterpolator* clone() = 0;

	protected:

		void clearLeft();
		void clearRight();

		void setRatioSamples(const int &samples);
		qreal ratio(const int &index) const;

	protected:

		ViSampleChunk *mLeftData;
		ViSampleChunk *mRightData;

	private:

		qreal mRatio;
		bool mDeleteLeft;
		bool mDeleteRight;
		int mLeftSamples;
		int mRightSamples;

};

#endif
