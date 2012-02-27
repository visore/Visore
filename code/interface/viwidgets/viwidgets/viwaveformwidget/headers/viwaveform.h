#ifndef ViWaveForm_H
#define ViWaveForm_H

#include "viwave.h"

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
		qint32 mCompression;
};

#endif
