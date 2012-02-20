#ifndef VIWAVECOLLECTION_H
#define VIWAVECOLLECTION_H

#define MAXIMUM_COMPRESSION_LEVELS 5
#define FIRST_COMPRESSION_LEVEL 20000

#include "viwave.h"

#include <iostream>
using namespace std;

class ViWaveCollection
{
	public:
		ViWaveCollection();
		~ViWaveCollection();
		void append(double value);
		int size(int level);
		int samples(int level);
		double maximum(int level, int position);
		double minimum(int level, int position);
		double maximumAverage(int level, int position);
		double minimumAverage(int level, int position);

	private:
		void update();
		void reset();

	private:
		QList<ViWave*> mWaves;
		QList<int> mSampleLevels;
		double mMaximum;
		double mMinimum;
		double mAverageMaximum;
		double mAverageMinimum;
		int mMaximumCounter;
		int mMinimumCounter;
		int mTotalCounter;
};

#endif
