#ifndef VINEARESTNEIGHBOURNOISEDETECTOR_H
#define VINEARESTNEIGHBOURNOISEDETECTOR_H

#include <vinoisedetector.h>

// ftp://cs.uku.fi/franti/papers/Hautamaki/P2.pdf
// http://www.ijcttjournal.org/volume-3/issue-2/IJCTT-V3I2P119.pdf

class ViNearestNeighbourNoiseDetector : public ViNoiseDetector
{

	public:

		enum OutlierMode
		{
			ODIN,
			MDIST,
			KDIST
		};

    public:

		ViNearestNeighbourNoiseDetector();
		ViNearestNeighbourNoiseDetector* clone();

		void setOutlierMode(const OutlierMode &mode);

		void setK(const int &k); // k nearest neighbours

	//protected:

		void calculateNoise(const ViSampleChunk &samples);

	private:

		void generateGraph(const ViSampleChunk &samples, QList<qreal> &vectors);

		void calculateOdin(const ViSampleChunk &samples);
		void calculateMdist(const ViSampleChunk &samples);
		void calculateKdist(const ViSampleChunk &samples);
		void (ViNearestNeighbourNoiseDetector::*calculatePointer)(const ViSampleChunk &samples);

	private:

		int mK;
		int mHalfK;
		QQueue<qreal> mCache;

};

#endif

