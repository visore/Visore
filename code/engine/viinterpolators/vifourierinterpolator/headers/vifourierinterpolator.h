#ifndef VIFOURIERINTERPOLATOR_H
#define VIFOURIERINTERPOLATOR_H

#include <viinterpolator.h>
#include <vivector.h>

// http://paulbourke.net/miscellaneous/interpolation/
// http://en.wikipedia.org/wiki/Trigonometric_interpolation
// http://online.redwoods.edu/instruct/darnold/laproj/fall2005/ken/Final_Paper.pdf
// http://www.muskingum.edu/~rdaquila/m350/fourier-interdn10-1.ppt‎
// http://www.cmi.ac.in/~arnabkar/courses/pdf/comp_phys/Lecture3.pdf
// http://www.cs.ntou.edu.tw/indexs/files/fslin/SC/SctcCh2st.pdf

// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

class ViFourierInterpolator : public ViInterpolator
{

	public:

		enum Estimation
		{
			Fixed,
			Best,
		};

		enum Mode
		{
			Normal,
			Osculating,	// Hermite-based: use additional derivatives
			Splines		// Use splines
		};

	public:

		ViFourierInterpolator(const Mode &mode = Normal, const Estimation &estimation = Fixed);
		ViFourierInterpolator(const ViFourierInterpolator &other);
		~ViFourierInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViFourierInterpolator* clone();

	protected:

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);
		bool validParameters(const Mode &mode, const int &leftSize, const int &rightSize, const int &degree, const int &derivatives);
		void setPointers(const Mode &mode, const Estimation &estimation);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);

		bool interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool interpolateBestDerivative(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);

		bool estimateModelNormal(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelNormal(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		bool estimateModelOsculating(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelOsculating(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		bool estimateModelSplines(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelSplines(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		void calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative, const int &offset, const int multiplier);
		qreal calculateMse(const qreal *observed, const qreal *interpolateed, const int &size);

	private:

		bool (ViFourierInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool (ViFourierInterpolator::*interpolateBestPointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool (ViFourierInterpolator::*estimateModelPointer)(const int &degree, const int &derivative, ViVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void (ViFourierInterpolator::*interpolateModelPointer)(const int &degree, const ViVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		Estimation mEstimation;
		Mode mMode;

		int mDegree;
		int mDerivatives;

};

#endif
