#ifndef VIPOLYNOMIALINTERPOLATOR_H
#define VIPOLYNOMIALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vieigen.h>

class ViPolynomialInterpolator : public ViInterpolator
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

		ViPolynomialInterpolator(const Mode &mode = Normal, const Estimation &estimation = Fixed);
		ViPolynomialInterpolator(const ViPolynomialInterpolator &other);
		~ViPolynomialInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViPolynomialInterpolator* clone();

	protected:

		void setType();

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);
		bool validParameters(const Mode &mode, const int &leftSize, const int &rightSize, const int &degree, const int &derivatives);
		void setPointers(const Mode &mode, const Estimation &estimation);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error = NULL);

		bool interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);
		bool interpolateFixedSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);

		bool interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);
		bool interpolateBestOsculating(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);
		bool interpolateBestSplines(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);

		ViEigenBaseVector* estimateModelNormal(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets = NULL);

		ViEigenBaseVector* estimateModelOsculating(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets = NULL);

		ViEigenBaseVector* estimateModelSplines(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets = NULL);

		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier);

		int decimalPrecision(const int &windowSize, const int &degree);

		void splineOffsetLeft(int *offsets, const int &size, const int &coefficientCount);
		void splineOffsetOutput(int *offsets, const int &size, const int &coefficientCount, const int &start);
		void splineOffsetRight(int *offsets, const int &size, const int &coefficientCount, const int &start, const int &outputSize);

	private:

		bool (ViPolynomialInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling, ViError *error = NULL);
		ViEigenBaseVector* (ViPolynomialInterpolator::*estimateModelPointer)(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void (ViPolynomialInterpolator::*interpolateModelPointer)(const int &degree, const ViEigenBaseVector *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling, const int *offsets = NULL);

		Estimation mEstimation;
		Mode mMode;

		int mDegree;
		int mDerivatives;

		ViEigenBase *mEigen;

};

#endif
