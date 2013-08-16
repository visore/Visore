#ifndef VIMODELSOLVER_H
#define VIMODELSOLVER_H

#include <vimatrix.h>
#include <vichunk.h>

// http://paulbourke.net/miscellaneous/ar/
// http://paulbourke.net/miscellaneous/ar/source/
// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

class ViModelSolver
{

	public:

		ViModelSolver();
		ViModelSolver(const ViModelSolver &other);
		virtual ~ViModelSolver();

		void clear();

		void setOrder(const int &order);
		int order() const;

		ViVector& coefficients();
		const ViVector& coefficients() const;

		qreal rss() const;

		bool estimate(const ViVector &samples);
		bool estimate(const ViSampleChunk &samples);

		static ViVector estimate(const int &order, const ViVector &samples);
		static ViVector estimate(const int &order, const ViSampleChunk &samples);

		static bool estimate(const int &order, const ViVector &samples, ViVector &coefficients);
		static bool estimate(const int &order, const ViSampleChunk &samples, ViVector &coefficients);

		static bool estimate(const int &order, const ViVector &samples, ViVector &coefficients, qreal &rss);
		static bool estimate(const int &order, const ViSampleChunk &samples, ViVector &coefficients, qreal &rss);

		static bool estimate(const int &order, const ViMatrix &matrix, const ViVector &vector, ViVector &coefficients);
		static ViVector estimate(const int &order, const ViMatrix &matrix, const ViVector &vector);

	private:

		static bool estimateCoefficients(const int &order, const ViVector &samples, ViMatrix &cached, ViMatrix &calculatedCached, ViVector &coefficients, qreal &rss);
		static bool estimateCoefficients(const int &order, const ViVector &samples, ViMatrix &cached, ViMatrix &calculatedCached, ViVector &coefficients);
		static bool estimateCoefficients(const int &order, const ViVector &samples, ViVector &coefficients);
		static bool estimateCoefficients(const int &order, const ViVector &samples, const ViMatrix &cached, ViVector &coefficients);

	private:

		int mOrder;
		ViMatrix mCacheMatrix;
		ViMatrix mCacheCalculatedMatrix;
		ViVector mCoefficients;
		qreal mRss;

};

#endif
