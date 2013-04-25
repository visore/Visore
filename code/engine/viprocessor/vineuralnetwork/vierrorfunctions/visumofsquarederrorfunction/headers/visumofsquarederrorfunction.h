#ifndef VISUMOFSQUAREDERRORFUNCTION_H
#define VISUMOFSQUAREDERRORFUNCTION_H

#include <vierrorfunction.h>

/*
	http://www.researchgate.net/publication/49620536_An_Analysis_of_the_Performance_of_Artificial_Neural_Network_Technique_for_Stock_Market_Forecasting/file/79e4150dc251fe7f69.pdf
	An Analysis of the Performance of Artificial Neural Network Technique for Stock Market Forecasting
	Page 2105

	http://upetd.up.ac.za/thesis/submitted/etd-02232007-095334/unrestricted/dissertation.pdf
	Neural networks for time series analysis
	Page 29
*/

class ViSumOfSquaredErrorFunction : public ViErrorFunction
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
