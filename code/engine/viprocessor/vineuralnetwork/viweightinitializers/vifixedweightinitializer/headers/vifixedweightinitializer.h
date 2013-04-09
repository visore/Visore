#ifndef VIFIXEDWEIGHTINITIALIZER_H
#define VIFIXEDWEIGHTINITIALIZER_H

#include <viweightinitializer.h>

/*
	http://www.elen.ucl.ac.be/Proceedings/esann/esannpdf/es2001-6.pdf
	Method 1
*/

class ViFixedWeightInitializer : public ViWeightInitializer
{

	public:

		ViFixedWeightInitializer();
		ViFixedWeightInitializer(const ViFixedWeightInitializer &other);
		virtual ~ViFixedWeightInitializer();

		void setValues(qreal value);
		void setValues(ViRealList values);
		void setValues(ViRealMatrix values);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViWeightInitializer* clone();

	protected:

		void clear();
		virtual void initializeWeight(ViSynapse *synapse);

	private:

		ViRealList mValues;
		int mCurrentValue;
		qreal mDefaultValue;

};

#endif
