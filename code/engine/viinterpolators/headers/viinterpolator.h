#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <vichunk.h>
#include <qmath.h>

class ViInterpolator : public ViLibrary
{

	public:

		ViInterpolator();
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		virtual void clear();

		// Takes ownership
		virtual void setData(ViSampleChunk *left, ViSampleChunk *right);
		virtual void setLeftData(ViSampleChunk *data);
		virtual void setRightData(ViSampleChunk *data);

		virtual qreal calculate() = 0;
		virtual ViSampleChunk calculate(int samples) = 0; //Number of samples to interpolate

		virtual ViInterpolator* clone() = 0;

	protected:

		void clearLeft();
		void clearRight();

	protected:

		ViSampleChunk *mLeftData;
		ViSampleChunk *mRightData;

};

#endif
