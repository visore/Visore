#ifndef VIFOURIERINTERPOLATOR_H
#define VIFOURIERINTERPOLATOR_H

#include <viinterpolator.h>

// http://paulbourke.net/miscellaneous/interpolation/
// http://en.wikipedia.org/wiki/Trigonometric_interpolation
// http://online.redwoods.edu/instruct/darnold/laproj/fall2005/ken/Final_Paper.pdf
// http://www.muskingum.edu/~rdaquila/m350/fourier-interdn10-1.ppt‎
// http://www.cmi.ac.in/~arnabkar/courses/pdf/comp_phys/Lecture3.pdf
// http://www.cs.ntou.edu.tw/indexs/files/fslin/SC/SctcCh2st.pdf

// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

class ViFourierInterpolator : public ViAutoDegreeInterpolator
{

	public:

		ViFourierInterpolator();
		ViFourierInterpolator(const int &degree);

		ViFourierInterpolator* clone();

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
