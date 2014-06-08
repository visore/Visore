#ifndef VIDIFFERENTIATER_H
#define VIDIFFERENTIATER_H

class ViDifferentiater
{

	public:

		// First parameter: 1 = first derivative, 2 = second, etc
		// For first derivative, first and last point doesn't have a derivative
		static double derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex, bool &error);

		// First and last points will use second and second-last (or further for higher derivatives) derivatives respectivly
		static double derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex);

		// Does not do any bound checks. Assumes points has the correct size
		// Der1 and der2 require 1 point on both sides. That means if points has a size of n, xIndex in [1, n-1]. For der3 and der4, we require 2 points on both sides. Etc ...
		// NB: Use with care
		static double derivative(const int &derivativeIndex, const double *points, const int &xIndex);

		// Calculates all derivatives and saves them to the last parameter
		static void derivative(const int &derivativeIndex, const double *points, const int &size, double *derivatives);

		// Calculates all derivatives for samples with a gap in the middle
		static void derivative(const int &derivativeIndex, const double *pointsLeft, const int &sizeLeft, const double *pointsRight, const int &sizeRight, const int &sizeGap, double *derivatives);
		static void derivative(const int &derivativeIndex, const double *pointsLeft, const int &sizeLeft, const double *pointsRight, const int &sizeRight, const int &sizeGap, double *derivativesLeft, double *derivativesRight);

	private:

		static inline double calculate(const int &derivativeIndex, const double *points, const int &xIndex);

};

#endif
