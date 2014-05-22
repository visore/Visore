#include <visystemsolver.h>
#include <vilogger.h>
#include <float.h>

ViVector ViSystemSolver::solve(const ViMatrix &matrix, const ViVector &vector)
{
	ViVector coefficients;
	solve(matrix, vector, coefficients);
	return coefficients;
}

bool ViSystemSolver::solve(const ViMatrix &matrix, const ViVector &vector, ViVector &coefficients)
{
	ViMatrix inverted;
	ViMatrix transpose = matrix.transpose();
	ViMatrix scalar = transpose.scalarMultiply(matrix);
	int row = 0, col = 0;
	if(!scalar.invert(inverted))
	{
		scalar[row][col] += DBL_EPSILON; // We increase with the smallest possible value if matrix is not invertible

		if(col < scalar.columns() - 1) ++col;
		else if(row < scalar.rows() - 1)
		{
			col = 0;
			++row;
		}
		else
		{
			coefficients.clear();
			STATICLOG("The matrix cannot be inverted.", QtCriticalMsg, "ViSystemSolver");
			return false;
		}
	}
	coefficients = inverted.scalarMultiply(transpose) * vector;
	return true;
}
