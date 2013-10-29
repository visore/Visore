#ifndef VINEARESTNEIGHBOURNOISEDETECTOR_H
#define VINEARESTNEIGHBOURNOISEDETECTOR_H

#include <vinoisedetector.h>

// ftp://cs.uku.fi/franti/papers/Hautamaki/P2.pdf
// http://www.ijcttjournal.org/volume-3/issue-2/IJCTT-V3I2P119.pdf

class ViNearestNeighbourNoiseDetector : public ViNoiseDetector
{

    public:

		ViNearestNeighbourNoiseDetector();
		ViNearestNeighbourNoiseDetector* clone();

		void setK(const int &k); // k nearest neighbours

	//protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mK;
		int mHalfK;

};

#endif

