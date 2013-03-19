#ifndef VIACTIVATIONFUNCTION_H
#define VIACTIVATIONFUNCTION_H

#include <viserializer.h>

/*
	Note: Subclasses must inheit from ViActivationFunctionType
*/

class ViActivationFunction : public ViSerializer
{

	public:

		ViActivationFunction(double functionMinimum = 0, double functionMaximum = 1);
		ViActivationFunction(const ViActivationFunction &other);
		virtual ~ViActivationFunction();

		virtual void setRange(double minimum, double maximum);
		virtual void setMinimum(double minimum);
		virtual void setMaximum(double maximum);

		double minimum() const;
		double maximum() const;

		double calculate(const double &input, const int &inputCount = -1);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViActivationFunction* clone() = 0;

	protected:

		virtual double execute(const double &input, const int &inputCount) = 0;

		void setFunctionRange(double minimum, double maximum);
		void setFunctionMinimum(double minimum);
		void setFunctionMaximum(double maximum);

	private:

		double mFunctionMinimum;
		double mFunctionMaximum;
		double mMinimum;
		double mMaximum;

};

#endif
