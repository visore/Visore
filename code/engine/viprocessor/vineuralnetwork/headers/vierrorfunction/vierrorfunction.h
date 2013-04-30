#ifndef VIERRORFUNCTION_H
#define VIERRORFUNCTION_H

#include <viserializer.h>
#include <vilibrary.h>

class ViErrorFunction : public ViLibrary, public ViSerializer
{

	public:

		ViErrorFunction();
		ViErrorFunction(const ViErrorFunction &other);
		virtual ~ViErrorFunction();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual qreal add(const qreal &realValue, const qreal &targetValue);
		virtual qreal add(const ViDoubleList &realValues, const ViDoubleList &targetValues);

		virtual void clear();
		virtual qreal error();

		virtual ViErrorFunction* clone() = 0;

	protected:

		int count();

		virtual qreal calculate(const qreal &realValue, const qreal &targetValue) = 0;
		virtual qreal calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues) = 0;

	private:

		qreal mError;
		int mCount;

};

#endif
