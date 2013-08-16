#ifndef VIMAZZONIMODELORDER_H
#define VIMAZZONIMODELORDER_H

#include <vimodelorder.h>

// Audacity - InterpolateAudio.cpp line 112
class ViMazzoniModelOrder : public ViModelOrder
{

	public:

		ViMazzoniModelOrder();
		ViMazzoniModelOrder(const ViMazzoniModelOrder &other);
		virtual ~ViMazzoniModelOrder();

		ViMazzoniModelOrder* clone();

		virtual void clear();

		void setBadSampleCount(const int &count);
		void setFirstBadSample(const int &index);

		int order() const;

		static int order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample);

	protected:

		int mBadSampleCount;
		int mFirstBadSample;

};

#endif
