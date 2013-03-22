#ifndef VIKIMRAWEIGHTINITIALIZER_H
#define VIKIMRAWEIGHTINITIALIZER_H

#include <viweightinitializer.h>

/*
	http://www.elen.ucl.ac.be/Proceedings/esann/esannpdf/es2001-6.pdf
	Method 2
*/

class ViKimRaWeightInitializer : public ViWeightInitializer
{

	public:

		ViKimRaWeightInitializer();
		ViKimRaWeightInitializer(const ViKimRaWeightInitializer &other);
		virtual ~ViKimRaWeightInitializer();

		void setUpperLimit(qreal upperLimit);

		qreal upperLimit();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViWeightInitializer* clone();

	protected:

		virtual void initialize(ViSynapse *synapse);

	private:

		qreal mUpperLimit;

};

#endif
