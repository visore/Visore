#ifndef VIFOURIERINTERPOLATOR_H
#define VIFOURIERINTERPOLATOR_H

#include <viinterpolator.h>
#include <visystemsolver.h>

// http://paulbourke.net/miscellaneous/interpolation/
// http://en.wikipedia.org/wiki/Trigonometric_interpolation
// http://online.redwoods.edu/instruct/darnold/laproj/fall2005/ken/Final_Paper.pdf
// http://www.muskingum.edu/~rdaquila/m350/fourier-interdn10-1.ppt‎
// http://www.cmi.ac.in/~arnabkar/courses/pdf/comp_phys/Lecture3.pdf
// http://www.cs.ntou.edu.tw/indexs/files/fslin/SC/SctcCh2st.pdf

// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

class ViFourierInterpolator : public ViDegreeInterpolator
{

	public:

		enum OrderSelection
		{
			Fixed,
			Best
		};

		ViFourierInterpolator();
		ViFourierInterpolator(const int &degree);

		ViFourierInterpolator* clone();

		void setOrderSelection(OrderSelection selection);

		void setParameters(const qreal &param1);
		void setParameters(const qreal &param1, const qreal &param2);

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		bool interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		bool estimate(const int &degree, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize);
		void interpolate(const int &degree, const ViVector &coefficients, const int &leftSize, const int &rightSize, qreal *outputSamples, const int &outputSize);
		qreal ess(const int &degree, const ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize);

	private:

		bool (ViFourierInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
