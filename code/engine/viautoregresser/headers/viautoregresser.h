#ifndef VIAUTOREGRESSER_H
#define VIAUTOREGRESSER_H

#include <vimatrix.h>
#include <vichunk.h>

// http://paulbourke.net/miscellaneous/ar/
// http://paulbourke.net/miscellaneous/ar/source/
// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

class ViAutoRegresser
{

	public:

		ViAutoRegresser();
		ViAutoRegresser(const ViAutoRegresser &other);
		virtual ~ViAutoRegresser();

		void clear();

		void setOrder(const int &order);
		int order() const;

		ViVector& coefficients();
		const ViVector& coefficients() const;

		qreal rss() const;

		bool calculate(const ViSampleChunk &samples);

	private:

		int mOrder;
		ViMatrix mCacheMatrix;
		ViMatrix mCacheCalculatedMatrix;
		ViVector mCoefficients;
		qreal mRss;

};

#endif
