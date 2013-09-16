#ifndef VITRIGINTERPOLATOR_H
#define VITRIGINTERPOLATOR_H

#include <viinterpolator.h>

// http://paulbourke.net/miscellaneous/interpolation/
// http://en.wikipedia.org/wiki/Trigonometric_interpolation
// http://online.redwoods.edu/instruct/darnold/laproj/fall2005/ken/Final_Paper.pdf
// http://www.muskingum.edu/~rdaquila/m350/fourier-interdn10-1.ppt‎
// http://www.cmi.ac.in/~arnabkar/courses/pdf/comp_phys/Lecture3.pdf
// http://www.cs.ntou.edu.tw/indexs/files/fslin/SC/SctcCh2st.pdf

// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

class ViTrigInterpolator : public ViInterpolator
{

	public:

		enum Mode
		{
			Sine,
			Cosine,
			Fourier
		};

	public:

		ViTrigInterpolator();
		ViTrigInterpolator(const Mode &mode);

		void setMode(const Mode &mode);
		Mode mode() const;

		ViTrigInterpolator* clone();

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		bool interpolateSamplesSine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateSamplesCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateSamplesFourier(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

	private:

		bool (ViTrigInterpolator::*interpolateSamplesPointer)(const qreal*, const int&, const qreal*, const int&, qreal*, const int&);

	private:

		Mode mMode;

};

#endif
