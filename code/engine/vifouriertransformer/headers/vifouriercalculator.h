#ifndef VIFOURIERCALCULATOR_H
#define VIFOURIERCALCULATOR_H

class ViFourierCalculator
{

	public:
		
		ViFourierCalculator();
		virtual void setData(double input[], double output[] = 0);
		virtual void setSize(int size);
		virtual int size();

		virtual void forward() = 0;
		virtual void inverse() = 0;
		virtual void rescale() = 0;

	protected:

		double *mInput;
		double *mOutput;
		int mSize;

};

#endif
