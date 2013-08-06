#ifndef VIAUTOREGRESSER_H
#define VIAUTOREGRESSER_H

#include <vimodelorder.h>
#include <vivector.h>
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

		void setModelOrder(ViModelOrder *modelOrder); //Takes ownership
		ViModelOrder* modelOrder();

		ViVector& coefficients();
		const ViVector& coefficients() const;

		bool calculate(const ViSampleChunk &samples);

	private:

		ViModelOrder *mModelOrder;
		ViVector mCoefficients;

};

#endif
