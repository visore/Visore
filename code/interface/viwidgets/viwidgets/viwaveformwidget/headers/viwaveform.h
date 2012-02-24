#ifndef ViWaveForm_H
#define ViWaveForm_H

#include "viwave.h"

#include <iostream>
using namespace std;

class ViWaveForm
{
	public:
		ViWaveForm();
		void append(qreal value);
		qint32 size();
		qreal maximum(qint32 position);
		qreal minimum(qint32 position);
		qreal maximumAverage(qint32 position);
		qreal minimumAverage(qint32 position);
		qreal setBufferSize(qint32 size);
		void setCompression(qint32 compression);
		void reset();
		void removeFirst();

	private:
		ViWave mWave;
		qreal mMaximum;
		qreal mMinimum;
		qreal mAverageMaximum;
		qreal mAverageMinimum;
		qint64 mMaximumCounter;
		qint64 mMinimumCounter;
		qint32 mTotalCounter;
		qint32 mBufferSize;
		qint32 mCompression;
};

#endif
