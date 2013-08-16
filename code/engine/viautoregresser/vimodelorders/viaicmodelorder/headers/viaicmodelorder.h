#ifndef VIAICMODELORDER_H
#define VIAICMODELORDER_H

#include <vimodelorder.h>
#include <QQueue>

// Akaike Information Criterion
// http://machinelearning102.pbworks.com/w/file/fetch/47699411/aic_reg.pdf
// http://pages.stern.nyu.edu/~churvich/TimeSeries/Handouts/AICC.pdf
// http://users.ecs.soton.ac.uk/jn2/teaching/aic.pdf
// http://www.brianomeara.info/tutorials/aic
// http://www.unc.edu/courses/2008fall/ecol/563/001/docs/lectures/lecture10.htm
// http://128.118.178.162/eps/comp/papers/0404/0404001.pdf

class ViAicModelOrder : public ViModelOrder
{

	public:

		ViAicModelOrder();
		ViAicModelOrder(const ViAicModelOrder &other);
		virtual ~ViAicModelOrder();

		ViAicModelOrder* clone();

		virtual void clear();

		void setRss(const qreal &rss);
		void addRss(const qreal &predictedValue, const qreal &realValue); //Continuesly adds new values and calculates the RSS. Reset with clear()

		int order() const;

		static int order(const int &sampleCount, const int &parameterCount, const qreal &rss);

	protected:

		qreal mRss;
		QQueue<qreal> mPredictedValues;
		QQueue<qreal> mRealValues;

};

#endif
