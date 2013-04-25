#ifndef VIMEANABSOLUTEERRORFUNCTION_H
#define VIMEANABSOLUTEERRORFUNCTION_H

#include <vierrorfunction.h>

/*
	http://www.eng.auburn.edu/~smithae/publications/bookch/janchap.pdf
	Page 5

	Chapter 4 in Artificial Neural Networks for Civil Engineers: Fundamentals and Applications
	(N. Kartam, I. Flood and J. Garrett, Editors), 1996 from ASCE Press
*/

class ViMeanAbsoluteErrorFunction : public ViErrorFunction
{

	public:

		ViErrorFunction* clone();

	protected:

		void clearValues();
		qreal calculate(const qreal &realValue, const qreal &targetValue);
		qreal calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues);

	private:

		qreal mSum;

};

#endif
