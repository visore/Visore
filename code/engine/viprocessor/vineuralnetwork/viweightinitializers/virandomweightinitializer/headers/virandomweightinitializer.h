#ifndef VIRANDOMWEIGHTINITIALIZER_H
#define VIRANDOMWEIGHTINITIALIZER_H

#include <viweightinitializer.h>

/*
	http://www.elen.ucl.ac.be/Proceedings/esann/esannpdf/es2001-6.pdf
	Method 1
*/

class ViRandomWeightInitializer : public ViWeightInitializer
{

	public:

		ViRandomWeightInitializer();
		ViRandomWeightInitializer(const ViRandomWeightInitializer &other);
		virtual ~ViRandomWeightInitializer();

		void setLimits(qreal lowerLimit, qreal upperLimit);
		void setLowerLimit(qreal lowerLimit);
		void setUpperLimit(qreal upperLimit);

		qreal lowerLimit();
		qreal upperLimit();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViWeightInitializer* clone();

	protected:

		virtual void initializeWeight(ViSynapse *synapse);

	private:

		qreal mLowerLimit;
		qreal mUpperLimit;

};

#endif
