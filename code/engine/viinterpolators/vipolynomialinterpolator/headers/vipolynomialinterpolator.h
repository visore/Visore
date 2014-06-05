#ifndef VIPOLYNOMIALINTERPOLATOR_H
#define VIPOLYNOMIALINTERPOLATOR_H

#include <viinterpolator.h>
#include <vieigen.h>

#define POLYNOMIAL_INTERPOLATOR_PRECISION 53
typedef ViEigen<POLYNOMIAL_INTERPOLATOR_PRECISION> EigenX;
typedef EigenX::Matrix ViEigenMatrix;
typedef EigenX::Vector ViEigenVector;

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

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);
		bool validParameters(const Mode &mode, const int &leftSize, const int &rightSize, const int &degree, const int &derivatives);
		void setPointers(const Mode &mode, const Estimation &estimation);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		bool interpolateFixed(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool interpolateBest(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);

		bool interpolateBestNormal(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool interpolateBestDerivative(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);

		ViEigenVectorZ* estimateModelNormal(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
				void interpolateModelNormal(const int &degree, const ViEigenVectorZ *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

	/*	bool estimateModelNormal(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelNormal(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		bool estimateModelOsculating(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelOsculating(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		bool estimateModelSplines(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void interpolateModelSplines(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		void calculateDerivative(const int &degree, const qreal &x, ViEigenMatrix &matrix, const int &rowOffset, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViEigenMatrix &matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier);
		qreal calculateMse(const qreal *observed, const qreal *interpolateed, const int &size);*/

		/*int decimalPlaces(const int &windowSize, const int &degree)
		{
			mpreal value(1, 1024);
			value = pow(value / windowSize, degree);
			QString valueString = QString::fromStdString(value.toString());
			int start = valueString.indexOf("e-");
			if(start < 0)
			{
				start = valueString.indexOf(",");
				if(start < 0) start = valueString.indexOf(".");
				if(start < 0) return 0;
				while(valueString.at(start + 1) == '0') ++start;
				return start;
			}
			else
			{
				return valueString.mid(start + 2).toInt();
			}
		}

		int bitPlaces(const int &windowSize, const int &degree)
		{
			int decimal = decimalPlaces(windowSize, degree);
			return ceil(decimal / log10(2));
		}*/

	private:

		/*bool (ViPolynomialInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool (ViPolynomialInterpolator::*interpolateBestPointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		bool (ViPolynomialInterpolator::*estimateModelPointer)(const int &degree, const int &derivative, ViEigenVector &coefficients, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
		void (ViPolynomialInterpolator::*interpolateModelPointer)(const int &degree, const ViEigenVector &coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);
*/

		bool (ViPolynomialInterpolator::*interpolatePointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
				bool (ViPolynomialInterpolator::*interpolateBestPointer)(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, const qreal &scaling);
		ViEigenVectorZ* (ViPolynomialInterpolator::*estimateModelPointer)(const int &degree, const int &derivative, const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, const qreal &scaling);
				void (ViPolynomialInterpolator::*interpolateModelPointer)(const int &degree, const ViEigenVectorZ *coefficients, qreal *outputSamples, const int &outputSize, const int &start, const qreal &scaling);

		Estimation mEstimation;
		Mode mMode;

		int mDegree;
		int mDerivatives;

		ViEigenBaseZ *mEigen;

};

#endif
