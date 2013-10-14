#ifndef VIWARPEDBURGINTERPOLATOR_H
#define VIWARPEDBURGINTERPOLATOR_H

#include <viinterpolator.h>
#include <vivector.h>

// http://lib.tkk.fi/Diss/2004/isbn9512269503/article3.pdf (INTERPOLATION OF LONG GAPS IN AUDIO SIGNALS USING THE WARPED BURG’S METHOD)
// http://emptyloops.com/technotes/A%20tutorial%20on%20Burg%27s%20method,%20algorithm%20and%20recursion.pdf
// http://dafx04.na.infn.it/WebProc/Proc/P_045.pdf
// http://www.acoustics.hut.fi/~esquef/mypapers/WASPAA03_Roth_errata.pdf
// http://www.academia.edu/905062/Frequency-warped_autoregressive_modeling_and_filtering

// http://www.acoustics.hut.fi/~mak/PUB/AES9167.pdf
// http://www.acoustics.hut.fi/~aqi/wwwPhD/P6.PDF

class ViWarpedBurgInterpolator : public ViDegreeInterpolator
{

	public:

		ViWarpedBurgInterpolator();

		ViWarpedBurgInterpolator* clone();

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

	private:

		void estimateBurg(const qreal &warpingFactor, const ViVector &samples, ViVector &coefficients);

		//double clalculateReflection();

};

#endif
