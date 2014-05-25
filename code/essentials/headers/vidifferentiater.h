#ifndef VIDIFFERENTIATER_H
#define VIDIFFERENTIATER_H

class ViDifferentiate
{

	public:

		// First parameter: 1 = first derivative, 2 = second, etc
		static double derivative(const int &derivativeIndex, const double *points, const int &size, const int &xIndex, bool &error);

		// Does not do any bound checks. Assumes points has the correct size
		// Der1 and der2 require 1 point on both sides. That means if points has a size of n, xIndex in [1, n-1]. For der3 and der4, we require 2 points on both sides. Etc ...
		static double derivative(const int &derivativeIndex, const double *points, const int &xIndex);

};

#endif
