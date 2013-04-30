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

		virtual void setData(ViSampleChunk *data); // Takes ownership

		virtual qreal calculate() = 0;

		virtual ViInterpolator* clone() = 0;

	private:

		ViSampleChunk *mData;

};

#endif
