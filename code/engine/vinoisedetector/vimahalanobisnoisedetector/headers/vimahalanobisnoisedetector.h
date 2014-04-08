#ifndef VIMAHALANOBISNOISEDETECTOR_H
#define VIMAHALANOBISNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vigretl.h>

// http://www.utexas.edu/courses/schwab/sw388r7/SolvingProblems/DetectingOutliers.ppt
// http://mathworld.wolfram.com/CovarianceMatrix.html
// http://feelmare.blogspot.com/2013/02/covariance-matrix-example.html

class ViMahalanobisNoiseDetector : public ViNoiseDetector
{

    public:

		ViMahalanobisNoiseDetector();
		ViMahalanobisNoiseDetector(const ViMahalanobisNoiseDetector &other);
		~ViMahalanobisNoiseDetector();
		ViMahalanobisNoiseDetector* clone();

		void setWindowSize(int size);

		void setParameters(qreal param1);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mWindowSize;
		DATASET *mGretlData;
		int *mGretlParameters;

};

#endif

