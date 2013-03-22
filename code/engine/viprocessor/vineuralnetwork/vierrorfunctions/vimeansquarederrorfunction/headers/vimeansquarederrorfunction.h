#ifndef VIMEANSQUAREDERRORFUNCTION_H
#define VIMEANSQUAREDERRORFUNCTION_H

#include <vierrorfunction.h>

/*
	http://www.eng.auburn.edu/~smithae/publications/bookch/janchap.pdf
	Page 5

	Chapter 4 in Artificial Neural Networks for Civil Engineers: Fundamentals and Applications
	(N. Kartam, I. Flood and J. Garrett, Editors), 1996 from ASCE Press
*/

class ViMeanSquaredErrorFunction : public ViErrorFunction
{

	public:

		ViErrorFunction* clone();

		qreal calculate(const ViRealMatrix &realValues, const ViRealMatrix &targetValues);

};

#endif
