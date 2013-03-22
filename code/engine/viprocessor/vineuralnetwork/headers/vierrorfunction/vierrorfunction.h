#ifndef VIERRORFUNCTION_H
#define VIERRORFUNCTION_H

#include <viserializer.h>

class ViErrorFunction : public ViSerializer
{

	public:

		ViErrorFunction();
		ViErrorFunction(const ViErrorFunction &other);
		virtual ~ViErrorFunction();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual void setRealValue(qreal realValue);
		virtual void setRealValues(ViRealList realValues);
		virtual void setRealValues(ViRealMatrix realValues);

		virtual void setTargetValue(qreal targetValue);
		virtual void setTargetValues(ViRealList targetValues);
		virtual void setTargetValues(ViRealMatrix targetValues);

		virtual qreal error();
		
		virtual qreal calculate();

		virtual qreal calculate(const ViRealMatrix &realValues, const ViRealMatrix &targetValues) = 0;

		virtual ViErrorFunction* clone() = 0;

	private:

		ViRealMatrix mRealValues;
		ViRealMatrix mTargetValues;
		qreal mError;

};

#endif
