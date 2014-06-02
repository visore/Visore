#ifndef VIEIGEN_H
#define VIEIGEN_H

#include <../../../external/eigen/Dense> // Must add file like this, because the Eigen "include" files have a relative path to the src directory
#include <../../../external/eigen/MPRealSupport>

using namespace Eigen;
using namespace mpfr;

typedef mpreal bigreal;

typedef Matrix<long double, Dynamic, Dynamic> ViEigenMatrix;
typedef Matrix<bigreal, Dynamic, Dynamic> ViEigenBigMatrix;
typedef Matrix<long double, Dynamic, 1> ViEigenVector;
typedef Matrix<bigreal, Dynamic, 1> ViEigenBigVector;


class ViEigen
{

	public:

		static void initialize();

		static ViEigenVector solve(const ViEigenMatrix &matrix, const ViEigenVector &vector);
		static void solve(const ViEigenMatrix &matrix, const ViEigenVector &vector, ViEigenVector &coefficients);

		static ViEigenBigVector solve(const ViEigenBigMatrix &matrix, const ViEigenBigVector &vector);
		static void solve(const ViEigenBigMatrix &matrix, const ViEigenBigVector &vector, ViEigenBigVector &coefficients);

		static long double toReal(const bigreal &real);

	private:

		static bool mInitialized;

};

#endif
