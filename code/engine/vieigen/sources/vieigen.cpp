#include <vieigen.h>

bool ViEigen::mInitialized = false;

void ViEigen::initialize()
{
	if(!mInitialized)
	{
		mInitialized = true;
		mpreal::set_default_prec(256);
	}
}

ViEigenVector ViEigen::solve(const ViEigenMatrix &matrix, const ViEigenVector &vector)
{
	return matrix.fullPivHouseholderQr().solve(vector);
}

void ViEigen::solve(const ViEigenMatrix &matrix, const ViEigenVector &vector, ViEigenVector &coefficients)
{
	coefficients = matrix.fullPivHouseholderQr().solve(vector);
}

ViEigenBigVector ViEigen::solve(const ViEigenBigMatrix &matrix, const ViEigenBigVector &vector)
{
	return matrix.fullPivHouseholderQr().solve(vector);
}

void ViEigen::solve(const ViEigenBigMatrix &matrix, const ViEigenBigVector &vector, ViEigenBigVector &coefficients)
{
	coefficients = matrix.fullPivHouseholderQr().solve(vector);
}

long double ViEigen::toReal(const bigreal &real)
{
	return real.toLDouble();
}
