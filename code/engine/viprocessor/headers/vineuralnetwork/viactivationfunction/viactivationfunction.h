#ifndef VIACTIVATIONFUNCTION_H
#define VIACTIVATIONFUNCTION_H

#include <viserializer.h>

class ViActivationFunction : public ViSerializer
{

	public:

		ViActivationFunction(double functionMinimum = 0, double functionMaximum = 1);
		virtual ~ViActivationFunction();

		void setName(QString name);
		QString name();

		virtual void setRange(double minimum, double maximum);
		virtual void setMinimum(double minimum);
		virtual void setMaximum(double maximum);

		double minimum();
		double maximum();

		double calculate(const double &input, const int &inputCount = -1);

	protected:

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual double execute(const double &input, const int &inputCount) = 0;

		void setFunctionRange(double minimum, double maximum);
		void setFunctionMinimum(double minimum);
		void setFunctionMaximum(double maximum);

	private:

		double mFunctionMinimum;
		double mFunctionMaximum;
		double mMinimum;
		double mMaximum;
		QString mName;

};

#endif
