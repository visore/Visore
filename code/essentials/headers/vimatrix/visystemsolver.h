#ifndef VISYSTEMSOLVER_H
#define VISYSTEMSOLVER_H

#include <vimatrix.h>

// http://paulbourke.net/miscellaneous/ar/
// http://paulbourke.net/miscellaneous/ar/source/
// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

class ViSystemSolver
{

	public:

		static ViVector solve(const ViMatrix &matrix, const ViVector &vector);
		static bool solve(const ViMatrix &matrix, const ViVector &vector, ViVector &coefficients);

};

#endif
